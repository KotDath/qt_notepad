#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent, const QString& fileName) : QSyntaxHighlighter(parent), doc("highlight") {
    QFile file(fileName);
    QFileInfo fi(fileName);
    auto extension = "cpp";
    if (!file.open(QIODevice::ReadOnly)) {
        doc.setContent(QByteArray());
    } else {
        doc.setContent(&file);
        file.close();
    }

    auto syntaxes = doc.documentElement().firstChild().childNodes();
    QDomElement* oneLineCommentElem = nullptr;
    QDomElement* multiLineCommentElem = nullptr;

    for (int i = 0; i < syntaxes.count(); ++i) {
        auto syntax = syntaxes.at(i).toElement();
        auto ext_list = syntax.attribute("ext_list");
        QStringList pieces = ext_list.split( "/" );
        if (pieces.contains(extension)) {
            auto nodes = syntax.childNodes();
            qDebug() << nodes.count();
            for (int j = 0; j < nodes.count(); ++j) {
                auto node = nodes.at(j);
                if (node.isComment()) {
                    auto comment = node.toComment().data();
                    if (comment == " Однострочный комментарий ") {
                        ++j;
                        if (j < nodes.count() && !oneLineCommentElem) {
                            oneLineCommentElem = new QDomElement(nodes.at(j).toElement());
                        }
                    } else if (comment == " Многострочный комментарий "){
                        ++j;
                        if (j < nodes.count() && !multiLineCommentElem) {
                            multiLineCommentElem = new QDomElement(nodes.at(j).toElement());
                        }
                    }


                } else {
                    addNode(node.toElement());
                }
            }
        }
    }

    qDebug() << rules.length();

    if (oneLineCommentElem) {
        addNode(*oneLineCommentElem);
        delete oneLineCommentElem;
    }

    if (multiLineCommentElem) {
        QTextCharFormat format;
        auto beginPattern = multiLineCommentElem->elementsByTagName("begin_pattern").at(0).toElement().attribute("value");
        auto endPattern = multiLineCommentElem->elementsByTagName("end_pattern").at(0).toElement().attribute("value");
        format.setFontWeight(multiLineCommentElem->elementsByTagName("format").at(0).toElement().attribute("font_weight").toInt());
        QColor color;
        color.setNamedColor(multiLineCommentElem->elementsByTagName("format").at(0).toElement().attribute("foreground"));
        format.setForeground(color);
        multiLineCommentFormat = format;
        commentEndExpression.setPattern(endPattern);
        commentStartExpression.setPattern(beginPattern);
        delete multiLineCommentElem;
    }
}

void Highlighter::highlightBlock(const QString& text) {
    qDebug() << text;
    for (auto& rule : rules) {
        QRegularExpressionMatchIterator i = rule.pattern.globalMatch(text);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    if (commentStartExpression.pattern() != "" && commentEndExpression.pattern() != "") {
        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1) {
            startIndex = text.indexOf(commentStartExpression);
        }

        while (startIndex >= 0) {
            QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
            int endIndex = match.capturedStart();
            int commentLength = 0;
            if (endIndex == -1) {
               setCurrentBlockState(1);
               commentLength = text.length() - startIndex;
            } else {
               commentLength = endIndex - startIndex + match.capturedLength();
           }
           setFormat(startIndex, commentLength, multiLineCommentFormat);
           startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
        }
    }
}

void Highlighter::addNode(const QDomElement& elem) {
    HighlightRule rule;
    auto patternList = elem.elementsByTagName("pattern");
    rule.format.setFontWeight(elem.toElement().elementsByTagName("format").at(0).toElement().attribute("font_weight").toInt());
    QColor color;
    color.setNamedColor(elem.toElement().elementsByTagName("format").at(0).toElement().attribute("foreground"));
    rule.format.setForeground(color);
    for (int k = 0; k < patternList.count(); ++k) {
        rule.pattern = QRegularExpression(patternList.at(k).toElement().attribute("value"));
        rules.append(rule);
    }
}
