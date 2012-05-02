#include "spellchecker.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

#include "hunspell.hxx"

SpellChecker::SpellChecker(const QString &dictionaryPath, const QString &userDictionary)
{
    _userDictionary = userDictionary;

    QString dictFile = dictionaryPath + ".dic";
    QString affixFile = dictionaryPath + ".aff";
    QByteArray dictFilePathBA = dictFile.toLocal8Bit();
    QByteArray affixFilePathBA = dictFile.toLocal8Bit();
    _hunspell = new Hunspell(dictFilePathBA.constData(), affixFilePathBA.constData());

    if(!_userDictionary.isEmpty()) {
        QFile userDictonaryFile(_userDictionary);
        if(userDictonaryFile.open(QIODevice::ReadOnly)) {
            QTextStream stream(&userDictonaryFile);
            for(QString word = stream.readLine(); !word.isEmpty(); word = stream.readLine())
                put_word(word);
            userDictonaryFile.close();
        } else {
            qWarning() << "User dictionary in " << _userDictionary << "could not be opened";
        }
    } else {
        qDebug() << "User dictionary not set.";
    }
}


SpellChecker::~SpellChecker()
{
    delete _hunspell;
}


bool SpellChecker::spell(const QString &word)
{
    // TODO UTF-8
    return _hunspell->spell(word.toLatin1().constData()) != 0;
}


QStringList SpellChecker::suggest(const QString &word)
{
    char **suggestWordList;

    // TODO UTF-8
    int numSuggestions = _hunspell->suggest(&suggestWordList, word.toLatin1().constData());
    QStringList suggestions;
    for(int i=0; i < numSuggestions; ++i) {
        suggestions << QString::fromLatin1(suggestWordList[i]);
        free(suggestWordList[i]);
    }
    return suggestions;
}


void SpellChecker::ignoreWord(const QString &word)
{
    put_word(word);
}


void SpellChecker::put_word(const QString &word)
{
    _hunspell->add(word.toLatin1().constData());
}


void SpellChecker::addToUserWordlist(const QString &word)
{
    put_word(word);
    if(!_userDictionary.isEmpty()) {
        QFile userDictonaryFile(_userDictionary);
        if(userDictonaryFile.open(QIODevice::Append)) {
            QTextStream stream(&userDictonaryFile);
            stream << word << "\n";
            userDictonaryFile.close();
        } else {
            qWarning() << "User dictionary in " << _userDictionary << "could not be opened for appending a new word";
        }
    } else {
        qDebug() << "User dictionary not set.";
    }
}
