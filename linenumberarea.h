#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include "codeeditor.h"

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) { //constructor
        codeEditor = editor;
    }

    QSize sizeHint() const { // gets width fo line number area
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) { // paints line numbers
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor; // the editor to apply the line number area to
};

#endif // LINENUMBERAREA_H
