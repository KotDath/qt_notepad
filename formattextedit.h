#ifndef FORMATTEXTEDIT_H
#define FORMATTEXTEDIT_H

#include <QTextEdit>
#include <QScrollBar>
#include <QFile>
#include <QWheelEvent>

class FormatTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit FormatTextEdit(QWidget *parent = nullptr);
protected:
    virtual void wheelEvent(QWheelEvent *e) override;

};

#endif // FORMATTEXTEDIT_H
