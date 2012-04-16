#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QPlainTextEdit>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>
#include <QString>
#include <QMessageBox>
#include <QStatusBar>
#include <QFileDialog>
#include <QCloseEvent>
#include <QTextCursor>
#include <QLayout>
#include <QSplitter>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTreeWidget>
#include <QActionGroup>
#include <QLabel>

#include "highlighter.h"
#include "linenumberarea.h"
#include "codeeditor.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


    void updateRecentFileActions();
    static QStringList recentFiles;
    static int currentScheme;


protected:
    void closeEvent(QCloseEvent *); //what happens when the window is closed?

private slots:
    void changeWindowModification();

    void newFile();
    void open();
    bool save();
    bool saveAs();
    void openRecentFile();
    void printf();

    void deletef();

    void about();


    void syntaxHighlight(bool);
    void doLineNumbers(bool);
    void doLineHighLight(bool);
    void lineWrapSlot(bool);
    void selectDarkColorScheme(bool x);
    void selectBlueColorScheme(bool);
    void selectStandardColorScheme(bool);
    void hideScratch(bool);
    void updateStatusBar();
  //void updateStatusBar();
  //void openTree();

private:
    enum { MaxRecentFiles = 5};

    bool loadFile(const QString &fileName);
    bool saveFile(const QString&);

    void readSettings();
    void setCurrentFile(const QString &fileName);
    void writeSettings();

    bool okToContinue();

    void createActions();
    void createMenus();
    void createToolBars();
  //void createContextMenu();
  //void createStatusBar();


    int is_highlighter;
    int lineNumbers;
    int syntax;
    int lineHighlight;
    int hidescratch;
    int linewrap;


    Highlighter *highlighter;

    QString strippedName(const QString &fullFileName);
    QString curFile;

    QPlainTextEdit *scratcharea1;
    QPlainTextEdit *scratcharea2;
    CodeEditor *textarea;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *optionsToolBar;
    QToolBar *toolsToolBar;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *selectSubMenu;
    QMenu *toolsMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QMenu *colorSchemeSubMenu;

    QAction *newAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction *openAction;
    QAction *separatorAction;
    QAction *selectAllAction;
    QAction *aboutQtAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;
    QAction *aboutAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *printAction;
    QAction *closeAction;
    QAction *cppSyntax;
    QAction *recentFileActions[MaxRecentFiles];
    QAction *lineNumberAction;
    QAction *lineHighLightAction;
    QAction *lineWrapAction;
    QAction *darkSchemeAction;
    QAction *blueSchemeAction;
    QAction *standardSchemeAction;
    QAction *hideScratchAction;

    QDockWidget *dock1;
    QDockWidget *dock2;

    QActionGroup *Schemes;

    QLabel *curLine;
    QLabel curColumn;
    //QDockWidget *files;

    //QSplitter *splitter2;

    //QFileSystemModel *model;
    //QTreeView *tree;
};

#endif // MAINWINDOW_H
