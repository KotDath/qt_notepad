#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QFile>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QTreeView>
#include <QToolBar>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QStatusBar>
#include <QDockWidget>

#include "editortabwidget.h"
#include "highlighter.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    virtual void closeEvent(QCloseEvent *event);
private slots:
    void setCurrentEdit(int index);
    void selectionChanged(const QItemSelection& newSelection);

    void cursorChanged();

    void openFile();
    void saveFile();
    void saveAs();
    void saveAllFiles();

    void openFileExplorer();
    void openedFilesExplorer();

    void fileExplorerSelection(QModelIndex index, QFileSystemModel* model);
private:
    void generateMenu();
    QMenu* generateFileMenu();
    QMenu* generateEditMenu();
    QMenu* generateViewMenu();

    EditorTabWidget* tabWidget;
    FormatTextEdit* currentEdit;

    QLabel* cursorLabel;
    Highlighter* highlighter;

    QAction* cutAction;
    QAction* copyAction;
    QAction* insertAction;
    QAction* deleteAction;
    QAction* selectAllAction;
};
#endif // MAINWINDOW_H
