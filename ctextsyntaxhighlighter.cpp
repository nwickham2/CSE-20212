#include "ctextsyntaxhighlighter.h"
#include "spellchecker.h"
#include <QTextBoundaryFinder>

extern SpellChecker *spellChecker;

CTextSyntaxHighlighter::CTextSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{

}
void CTextSyntaxHighlighter::highlightBlock(const QString &text)
{
    QTextCharFormat highlightFormat;
    highlightFormat.setUnderlineColor(QColor("red"));
    highlightFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);

//    qDebug(text.toAscii());

    QTextBoundaryFinder wordFinder(QTextBoundaryFinder::Word,text);
    int wordStart = 0;
    int wordLength = 0;
    QString wordValue = "";
    while (wordFinder.position() < text.length())
    {
        if (wordFinder.position()==0)
        {
            wordStart=0;
        }
        else
        {
            wordStart=wordFinder.position();
        }
        wordLength=wordFinder.toNextBoundary()-wordStart;

        wordValue=text.mid(wordStart,wordLength);

//        qDebug("pos_before:%d",wordFinder.position());
//        qDebug("->"+wordValue.toAscii()+"<-count:%d",wordValue.length());

        if(!spellChecker->spell(text.mid(wordStart,wordLength)))
            setFormat(wordStart, wordLength, highlightFormat);
    }
}
