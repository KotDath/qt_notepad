#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QFile>
#include <QPushButton>
#include <QStringListModel>
#include <QListWidget>
#include <QLabel>
#include <QFileDialog>

#include "editortabwidget.h"
#include "customdockwidget.h"
#include "highlighter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void addFile(const QString& fileName = "untitled");
    void removeFile();
    void setCurrentEdit(int index);
    void selectionChanged(const QItemSelection& newSelection);

    void cursorChanged();

    void openFile();
private:
    void generateMenu();
    QMenu* generateFileMenu();
    QMenu* generateEditMenu();
    QMenu* generateViewMenu();

    EditorTabWidget* tabWidget;
    FormatTextEdit* currentEdit;

    QStringListModel modelNames;
    QLabel* cursorLabel;
    Highlighter* highlighter;

    QAction* cutAction;
    QAction* copyAction;
    QAction* insertAction;
    QAction* deleteAction;
    QAction* selectAllAction;
};
#endif // MAINWINDOW_H
