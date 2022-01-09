#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QStringList>

class Highlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    Highlighter(QTextDocument* parent = nullptr, const QString& fileName = "");
    void setExtension();

protected:
    void highlightBlock(const QString& text) override;

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


};

#endif // HIGHLIGHTER_H
