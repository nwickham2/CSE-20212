#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>

#include "codeeditor.h"
#include "linenumberarea.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);
dolinenums = 0;
dolinelight = 0;
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

CodeEditor::CodeEditor(const QString & text, QWidget *parent) : QPlainTextEdit(text, parent)
{
    lineNumberArea = new LineNumberArea(this);
dolinenums = 0;
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

void CodeEditor::uninit()
{
updateLineNumberAreaWidth(-1);
highlightCurrentLine();
}

void CodeEditor::init()
{
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

void CodeEditor::setdolines(int x)
{
  dolinenums = x;
}

void CodeEditor::setdolinelight(int x)
{
    dolinelight = x;
}

int CodeEditor::lineNumberAreaWidth()
{
    if(dolinenums)
    {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
    }
    return 0;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{

    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(250);
        if(dolinelight)
        lineColor.setAlpha(50);
        else
            lineColor.setAlpha(0);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);

}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
 {
    if(dolinenums)
    {
     QPainter painter(lineNumberArea);
     painter.fillRect(event->rect(), BackgroundColor);
     QTextBlock block = firstVisibleBlock();
          int blockNumber = block.blockNumber();
          int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
          int bottom = top + (int) blockBoundingRect(block).height();
          while (block.isValid() && top <= event->rect().bottom()) {
              if (block.isVisible() && bottom >= event->rect().top()) {
                  QString number = QString::number(blockNumber + 1);
                  painter.setPen(NumbersColor);
                  painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                                   Qt::AlignRight, number);
              }

              block = block.next();
              top = bottom;
              bottom = top + (int) blockBoundingRect(block).height();
              ++blockNumber;
          }
    }
      }
void CodeEditor::setColors(int x)
{
    if(x ==1)
    {
BackgroundColor.setRgb(214, 214, 214);
NumbersColor.setRgb(255, 0, 255);
    }
    else if(x == 2)
    {
        BackgroundColor.setRgb(39,64,139);
        NumbersColor.setRgb(176, 196, 222);
    }
    else if(x == 3)
    {
        BackgroundColor.setRgb(218, 165, 32);
        NumbersColor.setRgb(139, 137, 137);
    }
}
