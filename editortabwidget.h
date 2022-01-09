#ifndef EDITORTABWIDGET_H
#define EDITORTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QDebug>
#include <QHBoxLayout>

#include "linecodenumwidget.h"
#include "formattextedit.h"

class EditorTabWidget : public QTabWidget {
    Q_OBJECT
public:
    EditorTabWidget(QWidget* parent = nullptr);
    FormatTextEdit* currentEdit();
private slots:
    void closeTab(int index);
public slots:
    void closeCurrentTab();
    void addEdit(const QString& fileName = "untitled");
    void closeAllTabs();
};

#endif // EDITORTABWIDGET_H
