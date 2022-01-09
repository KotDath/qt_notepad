#ifndef CUSTOMDOCKWIDGET_H
#define CUSTOMDOCKWIDGET_H

#include <QDockWidget>

class CustomDockWidget : public QDockWidget {
    Q_OBJECT
public:
    CustomDockWidget(const QString& title = "", QWidget* parent = nullptr);
};

#endif // CUSTOMDOCKWIDGET_H
