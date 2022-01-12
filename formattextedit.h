#ifndef FORMATTEXTEDIT_H
#define FORMATTEXTEDIT_H

#include <QTextEdit>
#include <QScrollBar>
#include <QFile>
#include <QWheelEvent>
#include <QFileDialog>
#include <QFileSystemWatcher>

class FormatTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit FormatTextEdit(QWidget *parent = nullptr, QFile* file = nullptr);
    bool IsSaved() {return isSaved;}
    void setSaved(bool a) {isSaved = a;}
    QString save();
    QString saveAs();

    QString getFileName();
    QString getPathName();
protected:
    virtual void wheelEvent(QWheelEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *e) override;
private slots:
    void handleFileChanged(const QString&);
signals:
    void fileChanged();
    void fileSaved();
    void zoomChanged();
    void wheeled();
private:
    bool isSaved;
    QFile* fileLink;
    QFileSystemWatcher watcher;

};

#endif // FORMATTEXTEDIT_H
