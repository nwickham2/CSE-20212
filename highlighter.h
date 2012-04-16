#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include <QColor>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0, int = 1);
    void setScheme(int);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat preprocessorFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat typeFormat;
    QTextCharFormat bracketFormat;

    QColor preprocessorColor;
    QColor typeColor;
    QColor keywordColor;
    QColor numberColor;
    QColor commentColor;
    QColor quotationColor;
    QColor bracketColor;
};

#endif // HIGHLIGHTER_H
