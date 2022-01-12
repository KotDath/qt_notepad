#ifndef LINECODENUMWIDGET_H
#define LINECODENUMWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QAbstractTextDocumentLayout>
#include <QTextBlock>

#include "formattextedit.h"

class LineCodeNumWidget : public QWidget {
    Q_OBJECT
public:
    explicit LineCodeNumWidget(QWidget *parent = nullptr);
    void setEdit(FormatTextEdit* edit_);
protected:
    virtual void paintEvent(QPaintEvent* e) override;
public slots:
    void cursorChanged();
    void zoomChanged();
    void wheeled(int value);
private:
    FormatTextEdit* edit;
    int currentRow = 1;
    int startRow = 1;
    int endRow = 1;
    int lineHeight = 12;
    int offset = 0;
    int prevValue = 0;

};

#endif // LINECODENUMWIDGET_H
