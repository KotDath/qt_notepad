#include "editortabwidget.h"

EditorTabWidget::EditorTabWidget(QWidget* parent) : QTabWidget(parent) {
    setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tabBar()->setDocumentMode(true);
    tabBar()->setExpanding(true);
    setTabPosition(QTabWidget::North);
    setMovable(true);
    setTabsClosable(true);

    QFile file(":/src/styles/tab_widget.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    setStyleSheet(styleSheet);

    connect(tabBar(), &QTabBar::tabCloseRequested, this, &EditorTabWidget::closeTab);

}

void EditorTabWidget::closeTab(int index) {
    auto currentWidget = widget(index);
    if (currentWidget) {
        currentWidget->deleteLater();
    }

}

void EditorTabWidget::addEdit(const QString& fileName) {
    auto field = new QWidget{this};
    auto mainField = new FormatTextEdit{field};
    mainField->setObjectName("edit_field");
    auto numField = new LineCodeNumWidget{field};
    numField->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    numField->setMinimumWidth(20);
    auto layout = new QHBoxLayout{field};
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(numField);
    layout->addWidget(mainField);
    addTab(field, fileName);
    setCurrentWidget(field);
}

void EditorTabWidget::closeCurrentTab() {
    auto curWidget = currentWidget();
    if (curWidget) {
        curWidget->deleteLater();
    }
}

void EditorTabWidget::closeAllTabs() {
    clear();
}
