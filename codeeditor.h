#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);
    explicit CodeEditor(const QString & text, QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event);

    
signals:
    
public slots:

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
         void highlightCurrentLine();
         void updateLineNumberArea(const QRect &, int);

private:
         QWidget *lineNumberArea;
    
};

#endif // CODEEDITOR_H
