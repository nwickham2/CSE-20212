#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QColor>
#include <QTextBlockUserData>

#include "ctextsyntaxhighlighter.h"

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);
    explicit CodeEditor(const QString & text, QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void setdolines(int);
    void setdolinelight(int);
    void init();
    void uninit();
    void setColors(int);
    void deleteHighlighter(bool);

protected:
    void resizeEvent(QResizeEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);


private:
    CTextSyntaxHighlighter *MyTextHighlighter;

signals:

public slots:
void matchParentheses();
private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void replaceWord();
    void addToDictionary();
    void updateTextSpeller();

private:
         bool matchLeftParenthesis(QTextBlock currentBlock, int index, int numRightParentheses);
         bool matchRightParenthesis(QTextBlock currentBlock, int index, int numLeftParentheses);
         void createParenthesisSelection(int pos);
         QWidget *lineNumberArea;
         int dolinenums;
         int dolinelight;
         QColor BackgroundColor;
         QColor NumbersColor;
         //CTextSyntaxHighlighter *MyTextHighlighter;
};


#endif // CODEEDITOR_H
