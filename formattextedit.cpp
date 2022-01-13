#include "formattextedit.h"

FormatTextEdit::FormatTextEdit(QWidget* parent, QFile* file)  : QTextEdit(parent), isSaved(false), fileLink(file) {
    QFile styleFile(":/src/styles/text_edit.qss");
    styleFile.open(QFile::ReadOnly);
    if (fileLink) {
        isSaved = true;
        watcher.addPath(file->fileName());
    }
    QString styleSheet = QLatin1String(styleFile.readAll());

    if (fileLink) {
        if (fileLink->open(QIODevice::ReadWrite)) {
            setPlainText(fileLink->readAll());
        } else if (fileLink->open(QIODevice::ReadOnly)) {
            setPlainText(fileLink->readAll());
            setReadOnly(true);
        } else {
            delete fileLink;
            fileLink = nullptr;
        }
    }

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setFrameStyle(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setStyleSheet(styleSheet);
    setLineWrapMode(QTextEdit::NoWrap);

   connect(&watcher, SIGNAL(fileChanged(const QString&)), this, SLOT(handleFileChanged(const QString&)));
}

void FormatTextEdit::wheelEvent(QWheelEvent* e) {
    auto angle = e->angleDelta();

    if (e->modifiers().testFlag(Qt::ControlModifier)) {
        if (angle.y() > 0) {
            zoomIn();
            emit zoomChanged();
        }

        if (angle.y() < 0) {
            zoomOut();
            emit zoomChanged();
        }
    } else {
        if (angle.y() != 0) {
            emit wheeled();
        }
        QTextEdit::wheelEvent(e);
    }
}

void FormatTextEdit::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Tab) {
        insertPlainText("    ");
    } else {
        QTextEdit::keyPressEvent(e);
        if (!e->modifiers().testFlag(Qt::ControlModifier)) {
            emit fileChanged();
        }
    }
}

QString FormatTextEdit::save() {
    if (fileLink) {
        fileLink->resize(0);
        fileLink->write(toPlainText().toUtf8());
        fileLink->flush();
        isSaved = true;
        return fileLink->fileName();
    } else {
        QString fileName = QFileDialog::getSaveFileName(this,
        tr("Сохранение файла"), "",
        tr("Любой файл (*)"));

        if (fileName != "") {
            fileLink = new QFile(fileName);
            fileLink->open(QIODevice::ReadWrite);
            fileLink->write(toPlainText().toUtf8());
            fileLink->flush();
            isSaved = true;
            watcher.addPath(fileName);
            return fileName;
        } else {
            return "";
        }

        return "untitled";
    }
}

QString FormatTextEdit::saveAs() {
    watcher.removePath(fileLink->fileName());
    fileLink->close();
    delete fileLink;
    QString fileName = QFileDialog::getSaveFileName(this,
    tr("Сохранение файла"), "",
    tr("Любой файл (*)"));

    if (fileName != "") {
        fileLink = new QFile(fileName);
        fileLink->open(QIODevice::ReadWrite);
        fileLink->write(toPlainText().toUtf8());
        fileLink->flush();
        isSaved = true;
        watcher.addPath(fileName);
        return fileName;
    } else {
        return "";
    }
}


QString FormatTextEdit::getFileName() {
    if (fileLink) {
        QFileInfo fileInfo(fileLink->fileName());
        QString filename(fileInfo.fileName());

        return filename;
    }

    return "untitled";
}

QString FormatTextEdit::getPathName() {
    if (fileLink) {
        return fileLink->fileName();
    }

    return "    "; //!!! Почему бы не {} просто
}


void FormatTextEdit::handleFileChanged(const QString& path) {
    QFile file{path};
    if (file.open(QFile::ReadWrite)) {
        setPlainText(file.readAll());
        file.close();
    }
}
