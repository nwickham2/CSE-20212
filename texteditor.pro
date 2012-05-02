#-------------------------------------------------
#
# Project created by QtCreator 2012-03-28T00:20:53
#
#-------------------------------------------------

QT       += core gui

TARGET = texteditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    highlighter.cpp \
    linenumberarea.cpp \
    codeeditor.cpp \
    utf_info.cxx \
    suggestmgr.cxx \
    spellchecker.cpp \
    replist.cxx \
    phonet.cxx \
    hunzip.cxx \
    hunspell.cxx \
    highlighter.cpp \
    hashmgr.cxx \
    filemgr.cxx \
    dictmgr.cxx \
    csutil.cxx \
    affixmgr.cxx \
    affentry.cxx

HEADERS  += mainwindow.h \
    highlighter.h \
    linenumberarea.h \
    codeeditor.h \
    w_char.hxx \
    suggestmgr.hxx \
    spellchecker.h \
    replist.hxx \
    phonet.hxx \
    langnum.hxx \
    hunzip.hxx \
    hunvisapi.h \
    hunspell.hxx \
    hunspell.h \
    htypes.hxx \
    highlighter.h \
    hashmgr.hxx \
    filemgr.hxx \
    dictmgr.hxx \
    csutil.hxx \
    baseaffix.hxx \
    atypes.hxx \
    affixmgr.hxx \
    affentry.hxx

OTHER_FILES += \
    userDict.txt \
    license.myspell \
    license.hunspell \
    en_US.dic \
    en_US.aff
