#ifndef EDITORTABWIDGET_H
#define EDITORTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QStringListModel>
#include <QTableWidget>
#include <QDialogButtonBox>
#include <QDialog>

#include "linecodenumwidget.h"
#include "formattextedit.h"

class EditorTabWidget : public QTabWidget {
    Q_OBJECT
public:
    EditorTabWidget(QWidget* parent = nullptr);
    QStringListModel* getModel() {return &modelNames;}

    bool fileSaved(int index);
    void removeTab(int index);
public slots:
    void addEdit(const QString& fileName);
    void addEmpty();
    void closeAllTabs();

private slots:
    void fileChanged();
private:

    void addFile(const QString& fileName = "untitled");
    void setFile(int index, const QString& fileName = "untitled");
    void removeFile(int index);

    QPixmap savedButton;
    QPixmap notSavedButton;
    QString styleSheet;

    QStringListModel modelNames;
};

#endif // EDITORTABWIDGET_H
