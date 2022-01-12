#include "editortabwidget.h"

EditorTabWidget::EditorTabWidget(QWidget* parent) : QTabWidget(parent), savedButton(":/close_button.png"), notSavedButton(":/not_saved_button.png") {
    setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tabBar()->setDocumentMode(true);
    tabBar()->setExpanding(true);
    setTabPosition(QTabWidget::North);
    setMovable(true);
    setTabsClosable(true);

    QFile file(":/src/styles/tab_widget.qss");
    file.open(QFile::ReadOnly);
    styleSheet = QLatin1String(file.readAll());

    setStyleSheet(styleSheet);
}

void EditorTabWidget::addEdit(const QString& fileName) {
    auto field = new QWidget{this};
    auto file = new QFile{fileName};
    auto mainField = new FormatTextEdit{field, file};
    mainField->setObjectName("edit_field");
    auto numField = new LineCodeNumWidget{field};
    numField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    numField->setMinimumWidth(50);
    numField->setStyleSheet("background-color: black;");
    auto layout = new QHBoxLayout{field};
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    //layout->addWidget(numField);
    layout->addWidget(mainField);
    addTab(field, fileName);
    setCurrentWidget(field);
    connect(mainField, SIGNAL(fileChanged()), this, SLOT(fileChanged()));
    addFile(fileName);

    numField->setEdit(mainField);
    connect(mainField, &FormatTextEdit::cursorPositionChanged, numField, &LineCodeNumWidget::cursorChanged);
    connect(mainField, SIGNAL(zoomChanged()), numField, SLOT(zoomChanged()));
    connect(mainField, SIGNAL(wheeled()), numField, SLOT(zoomChanged()));
    numField->cursorChanged();
    numField->zoomChanged();
}

void EditorTabWidget::addEmpty() {
    auto field = new QWidget{this};
    auto mainField = new FormatTextEdit{field};
    mainField->setObjectName("edit_field");
    auto numField = new LineCodeNumWidget{field};
    numField->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    numField->setMinimumWidth(150);
    auto layout = new QHBoxLayout{field};
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(numField);
    layout->addWidget(mainField);
    addTab(field, "untitled");
    setCurrentWidget(field);
    connect(mainField, SIGNAL(fileChanged()), this, SLOT(fileChanged()));
    addFile("untitled");

    numField->setEdit(mainField);
    connect(mainField, &FormatTextEdit::cursorPositionChanged, numField, &LineCodeNumWidget::cursorChanged);
    connect(mainField, SIGNAL(zoomChanged()), numField, SLOT(zoomChanged()));
    connect(mainField->verticalScrollBar(), SIGNAL(valueChanged(int)), numField, SLOT(wheeled(int)));
    numField->cursorChanged();
    numField->zoomChanged();

}

void EditorTabWidget::closeAllTabs() {
    QTableWidget canClose{};
    canClose.setColumnCount(2);
    for (int i = 0; i < count(); ++i) {
        auto curWidget = widget(i);
        auto currentEdit = curWidget->findChild<FormatTextEdit*>("edit_field");
        if (currentEdit->IsSaved()) {
            removeTab(i);
        } else {
            canClose.insertRow(i);
            auto first = new QTableWidgetItem{currentEdit->getFileName()};
            auto second = new QTableWidgetItem{currentEdit->getPathName()};
            canClose.setItem(i, 0, first);
            canClose.setItem(i, 1, second);
        }
    }

    auto dialog = new QDialog{};

    QVBoxLayout layout{dialog};

    canClose.setParent(dialog);
    layout.addWidget(&canClose);
    dialog->resize(800, 600);

    QDialogButtonBox buttons(QDialogButtonBox::Save | QDialogButtonBox::Discard);

    connect(&buttons, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

    layout.addWidget(&buttons);

    int ret = dialog->exec();

    switch (ret) {
        case QDialog::Accepted: {
            for (int i = 0; i < count(); ++i) {
                bool result = fileSaved(i);
                if (result) {
                    QTabWidget::removeTab(i);
                    removeFile(i);
                    --i;
                }
            }
            break;
        }
        case QDialog::Rejected:
            for (int i = 0; i < count(); ++i) {
                QTabWidget::removeTab(0);
                removeFile(0);
            }
            break;
        default:
            break;
    }

    dialog->deleteLater();
}

bool EditorTabWidget::fileSaved(int index) {
    auto curWidget = widget(index);
    auto currentEdit = curWidget->findChild<FormatTextEdit*>("edit_field");
    if (!currentEdit->IsSaved()) {
        QString newFileName = currentEdit->save();
        if (newFileName == "") {
            return false;
        } else {
            setTabText(index, newFileName);
            setFile(index, newFileName);
            return true;
        }

    }

    return false;
}

void EditorTabWidget::fileChanged() {
    auto currentEdit = currentWidget()->findChild<FormatTextEdit*>("edit_field");
    currentEdit->setSaved(false);
}

void EditorTabWidget::removeTab(int index) {
    auto currentEdit = currentWidget()->findChild<FormatTextEdit*>("edit_field");
    if (!currentEdit->IsSaved()) {
        QMessageBox msgBox;
        msgBox.setText(QString("Текст в файле %1 был изменён.").arg(tabText(index)));
        msgBox.setInformativeText("Вы хотите сохранить свои изменения?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
            case QMessageBox::Save: {
                bool result = fileSaved(index);
                if (result) {
                    QTabWidget::removeTab(index);
                    removeFile(index);
                }
                break;
            }
            case QMessageBox::Discard:
                QTabWidget::removeTab(index);
                removeFile(index);
                break;
            case QMessageBox::Cancel:
                break;
            default:
                break;
        }
    }
}

void EditorTabWidget::addFile(const QString& fileName) {
    auto fileNames = modelNames.stringList();
    fileNames.append(fileName);
    modelNames.setStringList(fileNames);
}

void EditorTabWidget::setFile(int index, const QString& fileName) {
    auto fileNames = modelNames.stringList();
    fileNames[index] = fileName;
    modelNames.setStringList(fileNames);
}

void EditorTabWidget::removeFile(int index) {
    auto fileNames = modelNames.stringList();
    if (index != -1) {
        fileNames.remove(index);
        modelNames.setStringList(fileNames);
    }
}
