#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H
/*
Nate Wickham
Dylan Zaragoza
Byron Zaragoza

highlighter.h
5/2/2012

highlighter.h defines the highlighter that is set to the text area is the cpp syntax option is toggled on.
It works primarily to define the rules that apply to the text area to highlight different words differently.
It also does th ebracket match highlighting.
*/

#include <QSyntaxHighlighter>
#include <QVector>
#include <QColor>
#include <QString>
#include <QTextDocument>


struct ParenthesisInfo //struct to hold the position and character for a bracket
{
    char character; // eitherr { or }
    int position; // the position
};

class TextBlockData : public QTextBlockUserData // the parentheses info for a block
{
public:
    TextBlockData();

    QVector<ParenthesisInfo *> parentheses();
    void insert(ParenthesisInfo *info);

private:
    QVector<ParenthesisInfo *> m_parentheses;
};

class Highlighter : public QSyntaxHighlighter // inherits QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0, int = 1);
    void setScheme(int); // sets the color scheme of the editor

protected:
    void highlightBlock(const QString &text); // pure virtual, must be defined

private:
    struct HighlightingRule // struct to defien a rule
    {
        QRegExp pattern; // regex pattern to find where to apply rule
        QTextCharFormat format; // format to apply to text fallign under rule
    };
    QVector<HighlightingRule> highlightingRules; // list for all the rules to be applied to the editor

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat; //all of the different formats
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat preprocessorFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat typeFormat;
    QTextCharFormat bracketFormat;

    QColor preprocessorColor; //all fo the different colars to be applied to the formats
    QColor typeColor;
    QColor keywordColor;
    QColor numberColor;
    QColor commentColor;
    QColor quotationColor;
    QColor bracketColor;
};

#endif // HIGHLIGHTER_H
