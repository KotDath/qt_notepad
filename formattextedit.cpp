#include "formattextedit.h"

FormatTextEdit::FormatTextEdit(QWidget* parent)  : QTextEdit(parent) {
    QFile file(":/src/styles/text_edit.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setFrameStyle(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setStyleSheet(styleSheet);
    setLineWrapMode(QTextEdit::NoWrap);
}

void FormatTextEdit::wheelEvent(QWheelEvent* e) {
    auto angle = e->angleDelta();

    if (e->modifiers().testFlag(Qt::ControlModifier)) {
        if (angle.y() > 0) {
            zoomIn();
        }

        if (angle.y() < 0) {
            zoomOut();
        }
    } else {
        QTextEdit::wheelEvent(e);
    }
}
