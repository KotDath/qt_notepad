#include "linecodenumwidget.h"

LineCodeNumWidget::LineCodeNumWidget(QWidget *parent) : QWidget(parent) {
}

void LineCodeNumWidget::setEdit(FormatTextEdit* edit_) {
    edit = edit_;
}

void LineCodeNumWidget::paintEvent(QPaintEvent* e) {
    QPainter painter{this};
    painter.setFont(edit->font());
    QRect geo = this->geometry();

    startRow = edit->cursorForPosition(QPoint(0, 5)).blockNumber() + 1;

    QPoint bottom_right(edit->viewport()->width() - 1, edit->viewport()->height() - 1);

    auto block = edit->cursorForPosition(bottom_right).block();
    endRow = edit->cursorForPosition(bottom_right).blockNumber()  + 1;

    painter.fillRect(0, 0, geo.width(), geo.height(), QColor(48,56,65));

    qDebug() << startRow << endRow;

    for (int i = startRow; i <= endRow; ++i) {
        if (i == currentRow) {
            painter.fillRect(0, (i - startRow) * lineHeight - offset, geo.width(), lineHeight, QColor(76,88,99));
        }

        painter.drawText(geo.width() / 2, lineHeight * (i - startRow + 1) - lineHeight / 4 - offset, QString::number(i));
    }
    QWidget::paintEvent(e);
}

void LineCodeNumWidget::cursorChanged() {
    currentRow = edit->textCursor().blockNumber() + 1;
    update();
}

void LineCodeNumWidget::zoomChanged() {
    auto block = edit->document()->findBlockByLineNumber(currentRow - 1);
    lineHeight = edit->document()->documentLayout()->blockBoundingRect(block).height();
    update();
}

void LineCodeNumWidget::wheeled(int value) {
    update();
    qDebug() << "Отступ" << offset;
    offset = value % lineHeight;
}
