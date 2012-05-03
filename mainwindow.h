#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/*
Nate Wickham
Dylan Zaragoza
Byron Zaragoza

mainwindow.h
5/2/2012

mainwindow.h is the primary driver of the text editor. It contains the finddialog, the codeeditor, and, through those, the other classes.
Mainwindow also has many built in functions.  Primarily, it has the menus, tootlbars, statusbar, scratchareas, file tree, and the primary text area.
It handles text input and output, all of the file I/O, and all of the actions. It also takes the key input.
*/

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
#include <QDir>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>

#include "highlighter.h"
#include "linenumberarea.h"
#include "codeeditor.h"
#include "finddialog.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0); //constructor
    ~MainWindow();


    void updateRecentFileActions(); // adds files to the recent files list inthe file menu
    static QStringList recentFiles; // list of recent files
    static int currentScheme; // numebr fo current colro scheme


protected:
    void closeEvent(QCloseEvent *); //what happens when the window is closed?
    void keyPressEvent(QKeyEvent *); // what happens when a key si pressed

private slots:
    void idontknow(QString, Qt::CaseSensitivity); // searches for a word going down the page
    void idontknow2(QString, Qt::CaseSensitivity); // searches for a word goign up the page
    void idontknow3(QString); // replaces the selected word
    void changeWindowModification(); //sets window modified variabel to true when etxtarea is changed

    void newFile(); // opens a new file/window
    void open(); // opens a fiel into the current text edit
    bool save(); //saves the current document
    bool saveAs(); //save the current document for the first time
    void openRecentFile(); //opens a file clicked on in the recen tfiles menu in the file menu
    void printf(); // ptinst the document

    void deletef(); //deletes the selected text

    void about(); // oepns a window with informaiton on the editor


    void syntaxHighlight(bool); // determeins whether or not to syntax highlight
    void doLineNumbers(bool); // determines whether or not to add lien numbers
    void doLineHighLight(bool); // determiens whether or not to highlight the current lien
    void lineWrapSlot(bool); // dtermeines if lines wrap
    void selectDarkColorScheme(bool x); // sets the current colro scheme to dark
    void selectBlueColorScheme(bool);// sets the current colro scheme to blue
    void selectStandardColorScheme(bool);// sets the current colro scheme to standard
    void hideScratch(bool); //determiens whther scratch areas are dispalyed
    void updateStatusBar(); // updates the sattus bar with line number, column number, and total number of lines
    void openTreeFile(); // opens the file clicked on in the tree
    void hidetree(bool); // determines whether or not the tree is displayed
    void treeUp(); // moves up a directory in the tree, sets this to the active directory
    void gotoDirect(); // moves the tree to a specified directory, sets thei directory to active
    void giveEditFocus(); // sets the cursor to the file tree lien edit
    void find(); // find a string
    void grammarHighlight(bool); // run spell check

private:
    enum { MaxRecentFiles = 5}; //sets the max numerb of recent files dispalyed

    bool loadFile(const QString &fileName); // loads a file of the anme fielName
    bool saveFile(const QString&); // saves a fiel with a specfied name

    void readSettings(); // reads the user's settings
    void setCurrentFile(const QString &fileName); //sets the name of the current file
    void writeSettings(); // write sthe settings to be used later by the user

    bool okToContinue(); //called when window is modified and user tries to close it.  prompsty to save

    void createActions(); // creates all fo the acitosn that may be called in the editor
    void createMenus(); // creates the menus in the editor
    void createToolBars(); // creates the tools bars in the editor


    int is_highlighter; //stores if the edit has an associated Highlighter
    int lineNumbers;//stores if lien numbers are active
    int syntax;//stores if syantax highlighting is active
    int lineHighlight; //stores if linehighlighter is active
    int hidescratch;//stores if scratch areas are shown
    int linewrap; // stores if leins wrap


    Highlighter *highlighter; // the highlighter to be applie dot the edit

    QString strippedName(const QString &fullFileName); // the name of the current file, without directory info
    QString curFile; // the current file name

    QPlainTextEdit *scratcharea1; // scratch area dispalye din dock widget
    QPlainTextEdit *scratcharea2; // scratch area dispalye din dock widget

    CodeEditor *textarea; // primary text edit area
    QActionGroup * toolOptions; // a group of radio actions to be used in the toolsMenu and toolbar


    QToolBar *fileToolBar; //the toolbars
    QToolBar *editToolBar;
    QToolBar *optionsToolBar;
    QToolBar *toolsToolBar;

    QMenu *fileMenu; // the menus
    QMenu *editMenu;
    QMenu *selectSubMenu;
    QMenu *toolsMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QMenu *colorSchemeSubMenu;

    QAction *newAction; // all of the actiosn the textedit can do
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
    QAction *hideFileTree;
    QAction *treeUpAction;
    QAction *gotoLineEdit;
    QAction *findAction;
    QAction *grammarHighlightAction;

    QDockWidget *dock1; //scratch area dock
    QDockWidget *dock2; // scratch area dock
    QDockWidget *downLocationDock; // lineedit for file tree movemnt dock
    QDockWidget *goButtonDock; // dock for label displaying current directory in file tree
    QDockWidget *files; // dock for fiel tree

    QActionGroup *Schemes; // makes the schemes radio, so only one can be applied at a time

    QLabel *curLine;// sets up display for status bar.  current line numerb and column number
    QLabel curColumn;

    QFileSystemModel *model; // model fo file system, used with file tree
    QListView *tree; // tree displayed in dock widget.  shows file system

    QDir *whereWeAre; // the actiev directory

    QLineEdit *downLocation; // inout widget for navigating fiel tree
    QWidget *widget; //widget with which the file tree is associated

    QVBoxLayout *fileLayout;
    FindDialog *dialog; // the find dialog, called when find is activiated
    QLabel *currentPath; // displayes the path of the active directory in the dock widget

    QStringList *commands; // a list of commands recently entered in the fiel tree edit

    int lastCommand; // the number fo the actiev command in the commands list

};

#endif // MAINWINDOW_H
