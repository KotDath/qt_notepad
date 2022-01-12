#ifndef FORMATTEXTEDIT_H
#define FORMATTEXTEDIT_H

#include <QTextEdit>
#include <QScrollBar>
#include <QFile>
#include <QWheelEvent>
#include <QFileDialog>

class FormatTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit FormatTextEdit(QWidget *parent = nullptr, QFile* file = nullptr);
    bool IsSaved() {return isSaved;}
    void setSaved(bool a) {isSaved = a;}
    QString save();

    QString getFileName();
    QString getPathName();
protected:
    virtual void wheelEvent(QWheelEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *e) override;
signals:
    void fileChanged();
    void fileSaved();
    void zoomChanged();
    void wheeled();
private:
    bool isSaved;
    QFile* fileLink;

};

#endif // FORMATTEXTEDIT_H
