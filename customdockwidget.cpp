#include "customdockwidget.h"

CustomDockWidget::CustomDockWidget(const QString& title, QWidget* parent) : QDockWidget(title, parent) {
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
}
