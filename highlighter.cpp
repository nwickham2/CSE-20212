/*
Nate Wickham
Dylan Zaragoza
Byron Zaragoza

highlighter.cpp
5/2/2012

highlighter.cpp defines the highlighter that is set to the text area is the cpp syntax option is toggled on.
It works primarily to define the rules that apply to the text area to highlight different words differently.
It also does th ebracket match highlighting.
*/

#include "highlighter.h"
#include "codeeditor.h"
#include <QTextBlockUserData>
#include <QColor>


TextBlockData::TextBlockData()
{
    // Nothing to do
}

QVector<ParenthesisInfo *> TextBlockData::parentheses()
{
    return m_parentheses;
}


void TextBlockData::insert(ParenthesisInfo *info)
{
    int i = 0;
    while (i < m_parentheses.size() &&
           info->position > m_parentheses.at(i)->position)
        ++i;

    m_parentheses.insert(i, info);
}

Highlighter::Highlighter(QTextDocument *parent, int x)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    setScheme(x);
    preprocessorFormat.setForeground(preprocessorColor);
    preprocessorFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("#[^\n]*");
    rule.format = preprocessorFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(quotationColor);
    QStringList quotationPatterns;
    quotationPatterns << "\'.*\'" << "\".*\"" << "<[A-Za-z0-9]+>";
    foreach(const QString &pattern, quotationPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = quotationFormat;
        highlightingRules.append(rule);
    }

    keywordFormat.setForeground(keywordColor);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bclass\\b" << "\\bconst\\b"
                    << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b" << "\\bvolatile\\b" <<"\\bfor\\b" << "\\bwhile\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
    //classFormat.setFontWeight(QFont::Bold);
    //   classFormat.setForeground(Qt::darkMagenta);
    //  rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    //  rule.format = classFormat;
    // highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(commentColor);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(commentColor);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");

    typeFormat.setForeground(typeColor);
    typeFormat.setFontWeight(QFont::Bold);
    QStringList typePatterns;
    typePatterns << "\\bint\\b" << "\\bchar\\b" << "\\bdouble\\b" << "\\benum\\b" << "\\blong\\b" << "\\bshort\\b" << "\\bvoid\\b";
    foreach(const QString &pattern, typePatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = typeFormat;
        highlightingRules.append(rule);
    }

    numberFormat.setForeground(numberColor);
    rule.pattern = QRegExp("\\b[0-9]+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    bracketFormat.setForeground(bracketColor);
    QStringList bracketPatterns;
    bracketPatterns << "[(){}]";
    foreach(const QString &pattern, bracketPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = bracketFormat;
        highlightingRules.append(rule);
    }





}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);
    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                    + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }

    TextBlockData *data = new TextBlockData;

    int leftPos = text.indexOf('{');
    while (leftPos != -1) {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = '{';
        info->position = leftPos;

        data->insert(info);
        leftPos = text.indexOf('{', leftPos + 1);
    }

    int rightPos = text.indexOf('}');
    while (rightPos != -1) {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = '}';
        info->position = rightPos;

        data->insert(info);

        rightPos = text.indexOf('}', rightPos +1);
    }

    setCurrentBlockUserData(data);
}

void Highlighter::setScheme(int x)
{

    if(x == 1)
    {
        preprocessorColor.setRgb(191, 62, 255);
        quotationColor.setRgb(255, 165, 0);
        keywordColor.setRgb(255, 255, 255);
        commentColor.setRgb(255, 20, 147);
        typeColor.setRgb(0, 205, 0);
        numberColor.setRgb(3, 168, 158);
        bracketColor.setRgb(119, 136, 153);
    }
    else if(x == 2)
    {
        preprocessorColor.setRgb(112, 219, 147);
        quotationColor.setRgb(255, 165, 0);
        keywordColor.setRgb(255, 140, 0);
        commentColor.setRgb(58, 95, 205);
        typeColor.setRgb(112, 219, 147);
        numberColor.setRgb(238, 0, 0);
        bracketColor.setRgb(180, 82, 205);
    }
    else if(x == 3)
    {
        preprocessorColor.setRgb(104, 24, 139);
        quotationColor.setRgb(46, 139, 87);
        keywordColor.setRgb(0, 34, 102);
        commentColor.setRgb(255, 140, 0);
        typeColor.setRgb(255, 20, 147);
        numberColor.setRgb(0, 238, 0);
        bracketColor.setRgb(180, 82, 205);
    }



}
