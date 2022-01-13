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
    numField->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    numField->setMinimumWidth(150);
    numField->setStyleSheet("background-color: black;");
    auto layout = new QHBoxLayout{field};
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(numField);
    layout->addWidget(mainField);
    QFileInfo fileInfo(fileName);
    QString shortName(fileInfo.fileName());
    addTab(field, shortName);
    setCurrentWidget(field);
    connect(mainField, SIGNAL(fileChanged()), this, SLOT(fileChanged()));
    addFile(shortName);

    numField->setEdit(mainField);
    connect(mainField, &FormatTextEdit::cursorPositionChanged, numField, &LineCodeNumWidget::cursorChanged);
    connect(mainField, SIGNAL(zoomChanged()), numField, SLOT(zoomChanged()));
    connect(mainField->verticalScrollBar(), SIGNAL(valueChanged(int)), numField, SLOT(wheeled(int)));
    numField->cursorChanged();
    numField->zoomChanged();

    auto closeButton = new QPushButton{};
    closeButtons.append(closeButton);
    closeButton->setStyleSheet("border: 0px; background-color: transparent;");
    closeButton->setIcon(savedButton);

    connect(closeButton, &QPushButton::pressed, this, [this, closeButton, mainField] {
        removeTab(closeButtons.indexOf(closeButton));
    });
    tabBar()->setTabButton(currentIndex(), QTabBar::RightSide, closeButton);
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

    auto closeButton = new QPushButton{};
    closeButtons.append(closeButton);
    closeButton->setStyleSheet("border: 0px; background-color: transparent;");
    closeButton->setIcon(notSavedButton);

    connect(closeButton, &QPushButton::pressed, this, [this, closeButton] {
        removeTab(closeButtons.indexOf(closeButton));
    });
    tabBar()->setTabButton(currentIndex(), QTabBar::RightSide, closeButton);

}

void EditorTabWidget::closeAllTabs() {
    auto dialog = new QWidget{};
    dialog->resize(800, 600);
    auto canClose = new QTableWidget{dialog};
    auto header = canClose->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    canClose->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    canClose->setColumnCount(2);
    for (int i = 0; i < count(); ++i) {
        auto curWidget = widget(i);
        auto currentEdit = curWidget->findChild<FormatTextEdit*>("edit_field");
        if (currentEdit->IsSaved()) {
            removeTab(i);
            --i;
        } else {
            canClose->insertRow(i);
            auto first = new QTableWidgetItem{currentEdit->getFileName()};
            auto second = new QTableWidgetItem{currentEdit->getPathName()};
            canClose->setItem(i, 0, first);
            canClose->setItem(i, 1, second);
        }
    }



    QVBoxLayout layout{dialog};
    layout.addWidget(canClose);

    auto buttons = new QWidget{dialog};
    layout.addWidget(buttons);
    auto buttonsLayout = new QHBoxLayout{buttons};

    auto acceptButton = new QPushButton{buttons};
    acceptButton->setText("Ок");
    buttonsLayout->addWidget(acceptButton);
    auto rejectButton = new QPushButton{buttons};
    buttonsLayout->addWidget(rejectButton);
    rejectButton->setText("Не сохранять");

    connect(rejectButton, &QPushButton::clicked, dialog, [this, dialog] {
        for (int i = 0; i < count(); ++i) {
            QTabWidget::removeTab(0);
            removeFile(0);
        }

        dialog->deleteLater();
    });

    connect(acceptButton, &QPushButton::clicked, dialog, [this, dialog] {
        for (int i = 0; i < count(); ++i) {
            bool result = fileSaved(i);
            if (result) {
                QTabWidget::removeTab(i);
                removeFile(i);
                --i;
            }

            dialog->deleteLater();
        }
    });

    if (canClose->rowCount() == 0) {
        dialog->deleteLater();

    } else {
        dialog->show();
    }
}

bool EditorTabWidget::fileSaved(int index) {
    auto curWidget = widget(index);
    auto currentEdit = curWidget->findChild<FormatTextEdit*>("edit_field");
    if (!currentEdit->IsSaved()) {
        QString newFileName = currentEdit->save();
        if (newFileName == "") {
            return false;
        } else {
            QFileInfo fileInfo(newFileName);
            QString filename(fileInfo.fileName());
            setTabText(index, filename);
            setFile(index, filename);

            auto closeButton = closeButtons.at(index);
            closeButton->setIcon(savedButton);

            return true;
        }

    }
    return false;
}

void EditorTabWidget::fileChanged() {
    auto currentEdit = currentWidget()->findChild<FormatTextEdit*>("edit_field");
    currentEdit->setSaved(false);

    auto closeButton = closeButtons.at(currentIndex());
    closeButton->setIcon(notSavedButton);
}

void EditorTabWidget::removeTab(int index) {
    auto currentEdit = widget(index)->findChild<FormatTextEdit*>("edit_field");
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
                    auto button = closeButtons.at(index);
                    closeButtons.remove(index);
                    button->deleteLater();
                    QTabWidget::removeTab(index);
                    removeFile(index);
                }
                break;
            }
            case QMessageBox::Discard: {
                auto button = closeButtons.at(index);
                closeButtons.remove(index);
                button->deleteLater();
                QTabWidget::removeTab(index);
                removeFile(index);
                break;
            }
            case QMessageBox::Cancel:
                break;
            default:
                break;
        }
    } else {
        auto button = closeButtons.at(index);
        closeButtons.remove(index);
        button->deleteLater();
        QTabWidget::removeTab(index);
        removeFile(index);
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

QString EditorTabWidget::saveAs(int index) {
    auto curWidget = widget(index);
    auto currentEdit = curWidget->findChild<FormatTextEdit*>("edit_field");
    QString newFileName = currentEdit->saveAs();
    if (newFileName != "") {
        QFileInfo fileInfo(newFileName);
        QString filename(fileInfo.fileName());
        setTabText(index, filename);
        setFile(index, filename);
        auto closeButton = closeButtons.at(index);
        closeButton->setIcon(savedButton);
        return newFileName;
    } else {
        return currentEdit->getPathName();
    }
}
