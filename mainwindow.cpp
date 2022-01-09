#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentEdit(nullptr), modelNames(), highlighter(new Highlighter{}) {

    auto mainWidget = new QWidget{this};
    setCentralWidget(mainWidget);

    setWindowTitle("Sublime Text");
    resize(800, 600);
    setWindowIcon(QIcon(":/logo.png"));

    mainWidget->setContentsMargins(0, 0, 0, 0);
    mainWidget->setStyleSheet("QWidget{background-color: rgb(114, 120, 126);}");
    auto mainVerticalLayout = new QVBoxLayout{mainWidget};
    mainVerticalLayout->setAlignment(Qt::AlignCenter);
    mainVerticalLayout->setSpacing(0);
    mainVerticalLayout->setContentsMargins(0,0,0,0);

    tabWidget = new EditorTabWidget{mainWidget};
    mainVerticalLayout->addWidget(tabWidget);

    auto additionalButtons = new QWidget{this};
    additionalButtons->setMinimumHeight(27);
    additionalButtons->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto horizontalAdditionalLayout = new QHBoxLayout{additionalButtons};
    horizontalAdditionalLayout->setSpacing(0);
    horizontalAdditionalLayout->setContentsMargins(6, 6, 6, 6);

    auto addFileButton = new QPushButton{additionalButtons};
    addFileButton->setStyleSheet("QPushButton {border: 0px; image: url(:/add_file_button.png);} QPushButton::hover{image: url(:/add_file_button_hover.png);}");
    addFileButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(addFileButton, &QPushButton::pressed, tabWidget, [this]() {tabWidget->addEdit();});
    connect(addFileButton, &QPushButton::pressed, this, [this]() {addFile();});



    horizontalAdditionalLayout->addWidget(addFileButton);


    auto allFilesButton = new QPushButton{additionalButtons};
    allFilesButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    allFilesButton->setStyleSheet("QPushButton {border: 0px; image: url(:/down_arrow_button.png);} QPushButton::hover{image: url(:/down_arrow_button_hover.png);}");
    horizontalAdditionalLayout->addWidget(allFilesButton);


    connect(tabWidget->tabBar(), &QTabBar::tabCloseRequested, this, &MainWindow::removeFile);
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::setCurrentEdit);
    tabWidget->setCornerWidget(additionalButtons, Qt::TopRightCorner);

    generateMenu();




    auto dock = new CustomDockWidget(tr("Открытые файлы"), this);
    auto customerList = new QListView(dock);
    customerList->setModel(&modelNames);
    dock->setWidget(customerList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    connect(customerList->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(selectionChanged(QItemSelection)));



    auto bottomWidget = new QWidget{mainWidget};
    bottomWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    bottomWidget->setFixedHeight(32);
    bottomWidget->setStyleSheet("QWidget {background-color: rgb(199, 203, 209);}");
    mainVerticalLayout->addWidget(bottomWidget);

    auto bottomHorizontalLayout = new QHBoxLayout{bottomWidget};
    cursorLabel = new QLabel{bottomWidget};
    cursorLabel->setText("Строка: 1 Столбец: 1");
    bottomHorizontalLayout->addWidget(cursorLabel);
}

MainWindow::~MainWindow()
{
}


void MainWindow::generateMenu() {
    menuBar()->addMenu(generateFileMenu());
    menuBar()->addMenu(generateEditMenu());
    menuBar()->addMenu(generateViewMenu());
}

QMenu* MainWindow::generateFileMenu() {
    auto fileMenu = new QMenu{tr("Файл")};

    auto newFileAction = new QAction{tr("Новый"), this};
    newFileAction->setShortcut(QKeySequence::New);
    fileMenu->addAction(newFileAction);
    connect(newFileAction, &QAction::triggered, tabWidget, [this]() {tabWidget->addEdit();});
    connect(newFileAction, &QAction::triggered, this, [this]() {addFile();});

    auto openAction = new QAction{tr("Открыть"), this};
    openAction->setShortcut(QKeySequence::Open);
    fileMenu->addAction(openAction);
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    auto saveAction = new QAction{tr("Сохранить"), this};
    saveAction->setShortcut(QKeySequence::Save);
    fileMenu->addAction(saveAction);

    auto saveAsAction = new QAction{tr("Сохранить как..."), this};
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    fileMenu->addAction(saveAsAction);

    auto saveAllAction = new QAction{tr("Сохранить все"), this};
    fileMenu->addAction(saveAllAction);

    auto closeAction = new QAction{tr("Закрыть"), this};
    closeAction->setShortcut(QKeySequence::Close);
    fileMenu->addAction(closeAction);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(removeFile()));
    connect(closeAction, SIGNAL(triggered()), tabWidget, SLOT(closeCurrentTab()));

    auto closeAllAction = new QAction{tr("Закрыть все"), this};
    fileMenu->addAction(closeAllAction);
    connect(closeAllAction, SIGNAL(triggered()), tabWidget, SLOT(closeAllTabs()));

    auto quitAction = new QAction{tr("Выход"), this};
    quitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(quitAction);

    return fileMenu;
}

QMenu* MainWindow::generateEditMenu() {
    auto editMenu = new QMenu{tr("Правка")};

    cutAction = new QAction{tr("Вырезать"), this};
    cutAction->setShortcut(QKeySequence::Cut);
    editMenu->addAction(cutAction);

    copyAction = new QAction{tr("Копировать"), this};
    copyAction->setShortcut(QKeySequence::Copy);
    editMenu->addAction(copyAction);

    insertAction = new QAction{tr("Вставить"), this};
    insertAction->setShortcut(QKeySequence::InsertLineSeparator);
    editMenu->addAction(insertAction);

    deleteAction = new QAction{tr("Удалить"), this};
    deleteAction->setShortcut(QKeySequence::Delete);
    editMenu->addAction(deleteAction);

    selectAllAction = new QAction{tr("Выделить все"), this};
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    editMenu->addAction(selectAllAction);

    return editMenu;
}

QMenu* MainWindow::generateViewMenu() {
    auto viewMenu = new QMenu{tr("Открыть")};

    auto showAction = new QAction{tr("Показать проводник"), this};
    viewMenu->addAction(showAction);

    auto showOpenDocumentAction = new QAction{tr("Показать обозреватель открытых документов"), this};
    viewMenu->addAction(showOpenDocumentAction);

    return viewMenu;
}

void MainWindow::addFile(const QString& fileName) {
    auto fileNames = modelNames.stringList();
    fileNames.append(fileName);
    modelNames.setStringList(fileNames);
}

void MainWindow::removeFile() {
    auto fileNames = modelNames.stringList();
    int index = tabWidget->currentIndex();
    if (index != -1) {
        fileNames.remove(index);
        modelNames.setStringList(fileNames);
    }
}

void MainWindow::setCurrentEdit(int index) {
    auto curWidget = tabWidget->currentWidget();
    if (currentEdit) {
        disconnect(cutAction, &QAction::triggered, currentEdit, &FormatTextEdit::cut);
        disconnect(copyAction, &QAction::triggered, currentEdit, &FormatTextEdit::copy);
        disconnect(insertAction, &QAction::triggered, currentEdit, &FormatTextEdit::paste);
        //disconnect(deleteAction, &QAction::triggered, currentEdit, &FormatTextEdit::);
        disconnect(selectAllAction, &QAction::triggered, currentEdit, &FormatTextEdit::selectAll);
        disconnect(currentEdit, &FormatTextEdit::cursorPositionChanged, this, &MainWindow::cursorChanged);
    }
    if (curWidget) {
        currentEdit = curWidget->findChild<FormatTextEdit*>("edit_field");
        cursorChanged();
        connect(cutAction, &QAction::triggered, currentEdit, &FormatTextEdit::cut);
        connect(copyAction, &QAction::triggered, currentEdit, &FormatTextEdit::copy);
        connect(insertAction, &QAction::triggered, currentEdit, &FormatTextEdit::paste);
        //connect(deleteAction, &QAction::triggered, currentEdit, &FormatTextEdit::);
        connect(selectAllAction, &QAction::triggered, currentEdit, &FormatTextEdit::selectAll);
        connect(currentEdit, &FormatTextEdit::cursorPositionChanged, this, &MainWindow::cursorChanged);

        highlighter->setDocument(currentEdit->document());
    } else {
        cursorLabel->setText("Строка: 1 Столбец: 1");
    }
}

void MainWindow::selectionChanged(const QItemSelection& newSelection) {
    int row = newSelection.indexes().first().row();
    tabWidget->setCurrentIndex(row);
}

void MainWindow::cursorChanged() {
    cursorLabel->setText(QString("Строка: %1 Столбец: %2").arg(currentEdit->textCursor().blockNumber() + 1).arg(currentEdit->textCursor().positionInBlock() + 1));
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),
                                                    QDir::homePath());

    if (fileName != "") {
        qDebug() << fileName;
        tabWidget->addEdit(fileName);
        addFile(fileName);
    }
}
