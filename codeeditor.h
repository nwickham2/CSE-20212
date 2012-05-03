#ifndef CODEEDITOR_H
#define CODEEDITOR_H

/*
Nate Wickham
Dylan Zaragoza
Byron Zaragoza

codeeditor.h
5/2/2012

codeeditor.h defines the actual text area.  It inherits QPlainTextEdit and all its functions.
It is sunbclassed primarily so that a spellchecker, line numer area, line highlighter, and bracket match highlighter can be run on the editor.
*/

#include <QPlainTextEdit>
#include <QColor>
#include <QTextBlockUserData>

#include "ctextsyntaxhighlighter.h"

class CodeEditor : public QPlainTextEdit //inherits QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);//constructors
    explicit CodeEditor(const QString & text, QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event); //draws the numbers in the line numebr area
    int lineNumberAreaWidth();// returns the width of th eline numebr area, matches size of highest visible line
    void setdolines(int); // determines is the line number area is visible
    void setdolinelight(int); //determines if the current line is highlighted
    void init();
    void uninit();
    void setColors(int); //sets the colors for the lien highlight and lien number area
    void deleteHighlighter(bool); // deletes the grammar highlighter

protected:
    void resizeEvent(QResizeEvent *event); // runs when codeditor window is resized
    void contextMenuEvent(QContextMenuEvent *event); //runs when user right clicks in codeeditor


private:
    CTextSyntaxHighlighter *MyTextHighlighter; //creates the grammar highlighter

signals:

public slots:
    void matchParentheses(); // finds the matching brackets
private slots:
    void updateLineNumberAreaWidth(int newBlockCount); // runs when new blocks are added
    void highlightCurrentLine(); //highlights the current line
    void updateLineNumberArea(const QRect &, int);
    void replaceWord(); //called form context menu
    void addToDictionary(); // called form context menu
    void updateTextSpeller(); // adds new words to grammar check

private:
    bool matchLeftParenthesis(QTextBlock currentBlock, int index, int numRightParentheses); // called by match parentheses to find left bracket
    bool matchRightParenthesis(QTextBlock currentBlock, int index, int numLeftParentheses); // called by match parentheses to fidn right bracket
    void createParenthesisSelection(int pos); // adds the posiiton fo brackets to extra selctions
    QWidget *lineNumberArea; // where the numbers are drawn
    int dolinenums; //sores if line numbers are active
    int dolinelight; // stores if lein highlgiht is active
    QColor BackgroundColor; // the color behind the numbes in the line number area
    QColor NumbersColor; // the color of th elien numbers
};


#endif // CODEEDITOR_H
