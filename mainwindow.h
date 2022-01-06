#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void generateMenu();
    QMenu* generateFileMenu();
    QMenu* generateEditMenu();
    QMenu* generateViewMenu();
};
#endif // MAINWINDOW_H
