#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QFileSystemWatcher>

class Highlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    Highlighter(QTextDocument* parent = nullptr, const QString& fileName = "");
    void setExtension(const QString& fileName);

protected:
    void highlightBlock(const QString& text) override;
private slots:
    void handleFileChanged(const QString&);

private:
    struct HighlightRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    void addNode(const QDomElement& elem);

    QDomDocument doc;
    QVector<HighlightRule> rules;

    QTextCharFormat multiLineCommentFormat;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QFileSystemWatcher watcher;

    QString currentFileName;
};

#endif // HIGHLIGHTER_H
