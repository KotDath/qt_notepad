#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    auto mainWidget = new QWidget{this};
    setCentralWidget(mainWidget);

    mainWidget->setContentsMargins(0, 0, 0, 0);
    mainWidget->setStyleSheet("QWidget{background-color: rgb(114, 120, 126);}");
    auto mainVerticalLayout = new QVBoxLayout{mainWidget};
    mainVerticalLayout->setAlignment(Qt::AlignCenter);
    mainVerticalLayout->setSpacing(0);
    mainVerticalLayout->setContentsMargins(0,0,0,0);

    generateMenu();
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

    auto openAction = new QAction{tr("Открыть"), this};
    openAction->setShortcut(QKeySequence::Open);
    fileMenu->addAction(openAction);

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

    auto closeAllAction = new QAction{tr("Закрыть все"), this};
    fileMenu->addAction(closeAllAction);

    auto quitAction = new QAction{tr("Выход"), this};
    quitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(quitAction);

    return fileMenu;
}

QMenu* MainWindow::generateEditMenu() {
    auto editMenu = new QMenu{tr("Правка")};

    return editMenu;
}

QMenu* MainWindow::generateViewMenu() {
    auto viewMenu = new QMenu{tr("Открыть")};

    return viewMenu;
}
