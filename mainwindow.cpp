#include "mainwindow.h"
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
#include <QTextStream>
#include <QFile>
#include <QApplication>
#include <QTextCursor>
#include <QPrintDialog>
#include <QPrinter>
#include <QSettings>
#include <QTextCharFormat>
#include <QDockWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QTreeWidget>
#include <QSplitter>
#include <iostream>
#include <QAbstractItemView>
#include <QPalette>
#include <QColor>
#include <QtDebug>
#include <QDir>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    MainWindow::currentScheme = 3;
textarea = new CodeEditor;
setCentralWidget(textarea);
setAttribute(Qt::WA_DeleteOnClose);
textarea->setLineWrapMode(QPlainTextEdit::NoWrap);
QPalette p = textarea->palette();
QColor backcolor(255, 255, 255);
p.setColor(QPalette::Base, backcolor);
QColor fontcolor(0, 0, 0);
p.setColor(QPalette::Text, fontcolor);
textarea->setPalette(p);
is_highlighter = 0;



splitter2 = new QSplitter;
model = new QFileSystemModel;
model->setRootPath(QDir::currentPath());
tree = new QTreeView(splitter2);
    tree->setModel(model);
    tree->setRootIndex(model->index(QDir::currentPath()));
         files = new QDockWidget(tr("Files"), this);
         addDockWidget(Qt::LeftDockWidgetArea, files);
         files->setWidget(splitter2);
         tree->setSelectionMode(QTreeView::SingleSelection);

createActions();
createMenus();
createToolBars();
readSettings();
setCurrentFile("");

if(MainWindow::currentScheme == 1)
    darkSchemeAction->setChecked(true);
    else if(MainWindow::currentScheme == 2)
    blueSchemeAction->setChecked(true);
    else if(MainWindow::currentScheme == 3)
    standardSchemeAction->setChecked(true);
/*
if(lineNumbers)
{
    lineNumberAction->setChecked(true);
}

    if(syntax)
    {
        cppSyntax->setChecked(true);
    }
    else cppSyntax->setChecked(false);

        if(lineHighlight)
        {
            lineHighLightAction->setChecked(true);
        }
        else lineHighLightAction->setChecked(false);

            if(hidescratch)
            {
                hideScratchAction->setChecked(true);
            }
            else hideScratchAction->setChecked(false);

                if(linewrap)
                {
                    lineWrapAction->setChecked(true);
                }
                else lineWrapAction->setChecked(false);
*/
                QString msg = QString( "Line Count: %1, Cursor Text Position: %2, column: %3" )
                        .arg( textarea->document()->lineCount() )
                        .arg( textarea->textCursor().blockNumber() +1 )
                        .arg( textarea->textCursor().columnNumber()+1 );

                    //statusBar()->showMessage( msg );
                curLine = new QLabel(msg);
                statusBar()->addWidget(curLine);

connect(textarea, SIGNAL(cursorPositionChanged()), this, SLOT(updateStatusBar()));

dock1 = new QDockWidget(tr("scratch area"), this);
dock2 = new QDockWidget(tr("scratch area"), this);
addDockWidget(Qt::BottomDockWidgetArea, dock1 );
addDockWidget(Qt::BottomDockWidgetArea, dock2 );
scratcharea1 = new QPlainTextEdit;
scratcharea2 = new QPlainTextEdit;
dock1 -> setWidget(scratcharea1);
dock2 -> setWidget(scratcharea2);
dock1->setFeatures(QDockWidget::NoDockWidgetFeatures);
dock2->setFeatures(QDockWidget::NoDockWidgetFeatures);
dock1->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
dock2->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
tabifyDockWidget(dock1, dock2);
/*
if(hidescratch)
{
    hideScratchAction->setChecked(true);
    removeDockWidget(dock1);
    removeDockWidget(dock2);
}
else
{hideScratchAction->setChecked(false);
    restoreDockWidget(dock1);
    restoreDockWidget(dock2);
}

*/
//createContextMenu();
//createStatusBar();

}

void MainWindow::updateStatusBar()
{
    QString msg = QString( "Line Count: %1, Cursor Text Position: %2, column: %3" )
            .arg( textarea->document()->lineCount() )
            .arg( textarea->textCursor().blockNumber()+1 )
            .arg( textarea->textCursor().columnNumber()+1 );

        statusBar()->showMessage( msg );

}

MainWindow::~MainWindow()
{
    
}

void MainWindow::createActions()
{
    connect(textarea, SIGNAL(textChanged()), this, SLOT(changeWindowModification()));

    newAction = new QAction(tr("&New"), this);
    newAction->setShortcut(tr("Ctrl+N"));
    newAction->setStatusTip(tr("Create a new document"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip(tr("Open an existing document"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip(tr("Save this document"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save As..."), this);
    saveAsAction->setShortcut(tr("F6"));
    saveAsAction -> setStatusTip(tr("Save this document as..."));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    closeAction = new QAction(tr("&Close"), this);
    closeAction->setShortcut(tr("Ctrl+W"));
    closeAction->setStatusTip(tr("Close this window"));
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

    printAction = new QAction (tr("&Print"), this);
    printAction->setShortcut(tr("Ctrl+P"));
    printAction->setStatusTip(tr("Print this document"));
    connect(printAction, SIGNAL(triggered()), this, SLOT(printf()));


    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));


    for (int i = 0; i < MaxRecentFiles; ++i)
    {
    recentFileActions[i] = new QAction(this);
    recentFileActions[i]->setVisible(false);
    connect(recentFileActions[i], SIGNAL(triggered()),
    this, SLOT(openRecentFile()));
    }

    selectAllAction = new QAction(tr("&All"), this);
    selectAllAction->setShortcut(tr("Ctrl+A"));
    selectAllAction->setStatusTip(tr("Select the entire document"));
    connect(selectAllAction, SIGNAL(triggered()), textarea, SLOT(selectAll()));

    cutAction = new QAction(tr("Cut"), this);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setStatusTip(tr("Delete the selected text and add it to the clipboard"));
    connect(cutAction, SIGNAL(triggered()), textarea, SLOT(cut()));

    copyAction = new QAction(tr("Copy"), this);
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setStatusTip(tr("Copy the selected text"));
    connect(copyAction, SIGNAL(triggered()), textarea, SLOT(copy()));

    pasteAction = new QAction(tr("Paste"), this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip(tr("Paste the most recent item copied to the clipboard"));
    connect(pasteAction, SIGNAL(triggered()), textarea, SLOT(paste()));

    deleteAction = new QAction(tr("Delete"), this);
    deleteAction->setShortcut(tr("Del"));
    deleteAction->setStatusTip(tr("Delete the selected text"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deletef()));

    undoAction = new QAction (tr("Undo"), this);
    undoAction->setShortcut(tr("Ctrl+Z"));
    undoAction->setStatusTip(tr("Undo the most recent action"));
    connect(undoAction, SIGNAL(triggered()), textarea->document(), SLOT(undo()));

    redoAction = new QAction (tr("Redo"), this);
    redoAction->setShortcut(tr("Ctrl+Y"));
    redoAction->setStatusTip(tr("Redoes the most recent undone action"));
    connect(redoAction, SIGNAL(triggered()), textarea->document(), SLOT(redo()));

    cppSyntax = new QAction(tr("&Highlight cpp"), this);
    cppSyntax->setCheckable(true);
    cppSyntax->setStatusTip(tr("Highlight the document with C++ syntax"));
    connect(cppSyntax, SIGNAL(toggled(bool)), this, SLOT(syntaxHighlight(bool)));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library?s About box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    aboutAction = new QAction(tr("About"), this);
    aboutAction->setStatusTip(tr("Show NotePad2's about box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    lineNumberAction = new QAction(tr("&Line Numbers"), this);
    lineNumberAction->setCheckable(true);
    lineNumberAction->setStatusTip(tr("Add line numbers"));
    connect(lineNumberAction, SIGNAL(toggled(bool)), this, SLOT(doLineNumbers(bool)));

    lineHighLightAction = new QAction(tr("&Line Highlight"), this);
    lineHighLightAction->setCheckable(true);
    //lineHighLightAction->setChecked(false);
    lineHighLightAction->setStatusTip(tr("Highlight current line"));
    connect(lineHighLightAction, SIGNAL(toggled(bool)), this, SLOT(doLineHighLight(bool)));

    lineWrapAction = new QAction(tr("Lines wrap"), this);
    lineWrapAction->setCheckable(true);
    lineWrapAction->setStatusTip(tr("Lines wrap"));
    connect(lineWrapAction, SIGNAL(toggled(bool)), this, SLOT(lineWrapSlot(bool)));

    darkSchemeAction = new QAction(tr("Dark color scheme"), this);
    darkSchemeAction->setCheckable(true);
    darkSchemeAction->setToolTip(tr("A color scheme utilizing dark tones"));
    connect(darkSchemeAction, SIGNAL(toggled(bool)), this, SLOT(selectDarkColorScheme(bool)));

    blueSchemeAction = new QAction(tr("Blue color scheme"), this);
    blueSchemeAction->setCheckable(true);
    blueSchemeAction->setToolTip(tr("A color scheme based on blues"));
    connect(blueSchemeAction, SIGNAL(toggled(bool)), this, SLOT(selectBlueColorScheme(bool)));

    standardSchemeAction = new QAction(tr("Standard color scheme"), this);
    standardSchemeAction->setCheckable(true);
    standardSchemeAction->setToolTip(tr("A basic color scheme"));
    connect(standardSchemeAction, SIGNAL(toggled(bool)), this, SLOT(selectStandardColorScheme(bool)));

    hideScratchAction = new QAction(tr("Hide scratch areas"), this);
    hideScratchAction->setCheckable(true);
    //hideScratchAction->setChecked (true);
    hideScratchAction->setToolTip(tr("Hides the scratch areas, but keeps the data intact"));
    connect(hideScratchAction, SIGNAL(toggled(bool)), this, SLOT(hideScratch(bool)));
  connect(tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openTreeFile()));

}


void MainWindow::openTreeFile()
{
    QModelIndexList selected = tree->selectionModel()->selectedIndexes();
    QFileInfo fileInfo = model->fileInfo(selected[0]);
    QString fileName = fileInfo.fileName();
    if(okToContinue())
    {
    loadFile(fileName);
    setCurrentFile(fileName);
    }
}


void MainWindow::hideScratch(bool x)
{
    if(x == true)
    {
removeDockWidget(dock1);
removeDockWidget(dock2);
hidescratch = 1;
    }
    else if(x == false)
    {
        restoreDockWidget(dock1);
    restoreDockWidget(dock2);
    hidescratch = 0;
    }
}

void MainWindow::selectDarkColorScheme(bool x)
{
    if(x == true)
    {
        if(cppSyntax->isChecked())
        {
            delete highlighter;
        MainWindow::currentScheme = 1;
        highlighter = new Highlighter(textarea->document(), MainWindow::currentScheme);
        }
        else
            MainWindow::currentScheme = 1;
        QPalette p = textarea->palette();
        QColor backcolor(38, 38, 38);
        p.setColor(QPalette::Base, backcolor);
        QColor fontcolor(214, 214, 214);
        p.setColor(QPalette::Text, fontcolor);
        textarea->setPalette(p);
        textarea->setColors(MainWindow::currentScheme);
}
}

void MainWindow::selectBlueColorScheme(bool x)
{
    if(x == true)
    {
        if(cppSyntax->isChecked())
        {
            delete highlighter;
        MainWindow::currentScheme = 2;
        highlighter = new Highlighter(textarea->document(), MainWindow::currentScheme);
        }
        else
            MainWindow::currentScheme = 2;
        QPalette p = textarea->palette();
        QColor backcolor(0, 30, 60);
        p.setColor(QPalette::Base, backcolor);
        QColor fontcolor(176, 196, 222);
        p.setColor(QPalette::Text, fontcolor);
        textarea->setPalette(p);
        textarea->setColors(MainWindow::currentScheme);
}
}

void MainWindow::selectStandardColorScheme(bool x)
{
    if(x == true)
    {
        if(cppSyntax->isChecked())
        {
            delete highlighter;
        MainWindow::currentScheme = 3;
        highlighter = new Highlighter(textarea->document(), MainWindow::currentScheme);
        }
        else
            MainWindow::currentScheme = 3;
        QPalette p = textarea->palette();
        QColor backcolor(255, 255, 255);
        p.setColor(QPalette::Base, backcolor);
        QColor fontcolor(0, 0, 0);
        p.setColor(QPalette::Text, fontcolor);
        textarea->setPalette(p);
        textarea->setColors(MainWindow::currentScheme);
}
}

void MainWindow::createMenus()
{
    fileMenu = menuBar() -> addMenu(tr("&File"));
    fileMenu -> addAction(newAction);
    fileMenu -> addAction(openAction);
    fileMenu -> addAction(saveAction);
    fileMenu -> addAction(saveAsAction);
    separatorAction = fileMenu -> addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);
    fileMenu -> addSeparator();
    fileMenu -> addAction(printAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction(closeAction);
    fileMenu -> addAction(exitAction);

    editMenu = menuBar() -> addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu -> addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);
    selectSubMenu = editMenu->addMenu(tr("&Select"));
    selectSubMenu->addAction(selectAllAction);
    editMenu->addSeparator();

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(cppSyntax);
    toolsMenu->addAction(lineNumberAction);
    toolsMenu->addAction(lineHighLightAction);

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(lineWrapAction);
    optionsMenu->addAction(hideScratchAction);
    colorSchemeSubMenu = optionsMenu->addMenu(tr("Color Scheme..."));

    Schemes = new QActionGroup(this);
    Schemes->addAction(darkSchemeAction);
    Schemes->addAction(blueSchemeAction);
    Schemes->addAction(standardSchemeAction);


    colorSchemeSubMenu->addAction(darkSchemeAction);
    colorSchemeSubMenu->addAction(blueSchemeAction);
    colorSchemeSubMenu->addAction(standardSchemeAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::createToolBars()
{
fileToolBar = addToolBar(tr("&File"));
fileToolBar->addAction(newAction);
fileToolBar->addAction(openAction);
fileToolBar->addAction(saveAction);
fileToolBar->addAction(printAction);

editToolBar = addToolBar(tr("&Edit"));
editToolBar->addAction(cutAction);
editToolBar->addAction(copyAction);
editToolBar->addAction(pasteAction);
editToolBar->addSeparator();
editToolBar->addAction(undoAction);
editToolBar->addAction(redoAction);

toolsToolBar = addToolBar(tr("&Tools"));
toolsToolBar->addAction(cppSyntax);
toolsToolBar->addAction(lineNumberAction);
toolsToolBar->addAction(lineHighLightAction);
addToolBarBreak(Qt::TopToolBarArea);
optionsToolBar = addToolBar(tr("&Options"));
optionsToolBar->addAction(lineWrapAction);
optionsToolBar->addAction(hideScratchAction);
//editToolBar->addAction(findAction);
//editToolBar->addAction(goToCellAction);
}

void MainWindow::changeWindowModification()
{if(!textarea->document()->isEmpty())
    setWindowModified(1);
}

void MainWindow::printf()
{
    QTextDocument *document = textarea->document();
         QPrinter printer;

         QPrintDialog *dlg = new QPrintDialog(&printer, this);
         if (dlg->exec() != QDialog::Accepted)
             return;

         document->print(&printer);
}

void MainWindow::deletef()
{
    QTextCursor cursor = textarea -> textCursor();
    cursor.deleteChar();
}

void MainWindow::syntaxHighlight(bool x)
{
    if(x == true)
    {
        syntax = 1;
        is_highlighter = 1;
        connect(textarea, SIGNAL(cursorPositionChanged()),
                textarea, SLOT(matchParentheses()));
        highlighter = new Highlighter(textarea->document(), MainWindow::currentScheme);
textarea->matchParentheses();
  //connect(this, SIGNAL(cursorPositionChanged()), highlighter, SLOT(rehighlightBlock));
    }
    else
    {
        syntax = 0;
        delete highlighter;
        is_highlighter = 0;
        disconnect(textarea, SIGNAL(cursorPositionChanged()),
                textarea, SLOT(matchParentheses()));
          //connect(this, SIGNAL(cursorPositionChanged()), highlighter, SLOT(rehighlightBlock));
    }
}

void MainWindow::lineWrapSlot(bool x)
{
    if(x == true)
    {
        textarea->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        linewrap = 1;
    }
        else
    {
        textarea->setLineWrapMode(QPlainTextEdit::NoWrap);
        linewrap = 0;
    }


}

void MainWindow::doLineHighLight(bool x)
{
    if(x == true)
    {
        lineHighlight = 1;
        textarea->setdolinelight(1);
        textarea->init();
    }
    else
    {
        lineHighlight = 0;
        textarea->setdolinelight(0);
        textarea->uninit();
    }
}

void MainWindow::doLineNumbers(bool x)
{
  /*  if(is_highlighter)
        delete highlighter;
    QTabWidget::TabPosition order;
    order = tabPosition(Qt::BottomDockWidgetArea);
    QString temp, temp1, temp2;
    QPlainTextEdit *holder = new QPlainTextEdit;
    setCentralWidget(holder);
    temp = textarea->toPlainText();
    temp1 = scratcharea1->toPlainText();
    temp2 = scratcharea2->toPlainText();*/
    if(x == true)
    {
        lineNumbers = 1;
        textarea->setdolines(1);
        textarea->init();
      //  delete textarea;
     //   textarea = new CodeEditor(temp);
    }
    else
    {
        lineNumbers = 0;
        textarea->setdolines(0);
        textarea->uninit();
      //  delete textarea;
      //  textarea = new QPlainTextEdit(temp);
    }
   /* setCentralWidget(textarea);
    createActions();

    if(is_highlighter)
        highlighter = new Highlighter(textarea->document());
    delete dock1;
    delete dock2;
    dock1 = new QDockWidget(tr("scratch area"), this);
    dock2 = new QDockWidget(tr("scratch area"), this);
    addDockWidget(Qt::BottomDockWidgetArea, dock1 );
    addDockWidget(Qt::BottomDockWidgetArea, dock2 );
    scratcharea1 = new QPlainTextEdit(temp1);
    scratcharea2 = new QPlainTextEdit(temp2);
    dock1 -> setWidget(scratcharea1);
    dock2 -> setWidget(scratcharea2);
    dock1->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dock2->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dock1->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    dock2->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    tabifyDockWidget(dock1, dock2);
    setTabPosition(Qt::BottomDockWidgetArea, order);
    QPalette p = textarea->palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::magenta);
    textarea->setPalette(p);
    textarea->setLineWrapMode(QPlainTextEdit::NoWrap);*/
}


void MainWindow::newFile()
{
MainWindow *mainWin = new MainWindow;
mainWin->show();
}

bool MainWindow::okToContinue()
{
    if(isWindowModified())
    {
        int r = QMessageBox::warning(this, tr("QTextEdit"), tr("The document has been modified.\n""Do you want to save your changes?"), QMessageBox::Yes | QMessageBox::Default, QMessageBox::No, QMessageBox::Cancel | QMessageBox::Escape);
        if (r == QMessageBox::Yes)
        {
            return save();
        }
        else if (r == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

void MainWindow::open()
{
    if (okToContinue())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Document"), ".", tr("Document files(*.txt *.cpp *.h *.java);;Text files(*.txt);;C++ source code (*.cpp);;C++ header files(*.h);; All files(*)"));
        if(!fileName.isEmpty())
            loadFile(fileName);
        setCurrentFile(fileName);
    }
}
bool MainWindow::loadFile(const QString &fileName)
{

    QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           {
            statusBar() -> showMessage(tr("Loading cancelled"), 2000);
            return false;
        }

        /*QFile file("box.txt");
 if (file.open(QFile::ReadOnly)) {
     char buf[1024];
     qint64 lineLength = file.readLine(buf, sizeof(buf));
     if (lineLength != -1) {
         // the line is available in buf
     }
 } */
textarea->clear();
QPalette p = textarea->palette();
QColor color = p.color(QPalette::Active, QPalette::Text);
color.setAlpha(0);
p.setColor(QPalette::Text, color);
textarea->setPalette(p);
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            textarea->appendPlainText(line);
            textarea -> moveCursor (QTextCursor::Start) ;
        }
    setCurrentFile(fileName);
    statusBar() -> showMessage(tr("File loaded"), 2000);
    textarea -> moveCursor (QTextCursor::Start) ;
    textarea -> ensureCursorVisible() ;
    color.setAlpha(255);
    p.setColor(QPalette::Text, color);
    textarea->setPalette(p);
    QFileInfo info(file);
    QString string = info.absolutePath();

tree->setRootIndex(model->index(string));

    return true;
}

bool MainWindow::save()
{
    if(curFile.isEmpty())
        return saveAs();
    else
        return saveFile(curFile);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        statusBar() -> showMessage(tr("Saving cancelled"), 2000);
        return false;
    }

QByteArray temp;
temp += textarea->toPlainText();
file.write(temp);
    setCurrentFile(fileName);
    statusBar() -> showMessage(tr("File saved"), 2000);
    return true;
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Document"), ".", tr("Document files(*.txt *.cpp *.h *.java);;Text files(*.txt);;C++ source code (*.cpp);;C++ header files(*.h);;All files(*)"));
    if (fileName.isEmpty())
        return false;
    setCurrentFile(fileName);
    return saveFile(fileName);
}

void MainWindow::setCurrentFile(const QString &fileName)
{
curFile = fileName;
setWindowModified(false);
QString shownName = "Untitled";
if (!curFile.isEmpty())
{
shownName = strippedName(curFile);
MainWindow::recentFiles.removeAll(curFile);
MainWindow::recentFiles.prepend(curFile);
foreach (QWidget *win, QApplication::topLevelWidgets())
{
if (MainWindow *mainWin = qobject_cast<MainWindow *>(win))
mainWin->updateRecentFileActions();
}
}
setWindowTitle(tr("%1[*] - %2").arg(shownName)
.arg(tr("NotePad2")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
return QFileInfo(fullFileName).fileName();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    writeSettings();
    if(okToContinue())
        e->accept();
    else
        e->ignore();
}

void MainWindow::updateRecentFileActions()
{
QMutableStringListIterator i(MainWindow::recentFiles);
while (i.hasNext()) {
if (!QFile::exists(i.next()))
i.remove();
}
for (int j = 0; j < MaxRecentFiles; ++j)
{
if (j < MainWindow::recentFiles.count())
{
QString text = tr("&%1 %2")
.arg(j + 1)
.arg(strippedName(MainWindow::recentFiles[j]));
recentFileActions[j]->setText(text);
recentFileActions[j]->setData(MainWindow::recentFiles[j]);
recentFileActions[j]->setVisible(true);
}
else
{
recentFileActions[j]->setVisible(false);
}
}
separatorAction->setVisible(!MainWindow::recentFiles.isEmpty());
}

void MainWindow::openRecentFile()
{
if (okToContinue())
{
QAction *action = qobject_cast<QAction *>(sender());
if (action)
loadFile(action->data().toString());
}
}

void MainWindow::about()
{
QMessageBox::about(this, tr("About NotePad2"),
tr("NotePad2 1.1"
"Not yet copyrighted"
"NotePad2 is a small text editing application that "
"created to fulfill the project requirement for "
"a Fundementals of Computing class at the "
"University of Notre Dame."));
}

void MainWindow::writeSettings()
{
QSettings settings("FundComp", "NotePad2");
settings.setValue("geometry", geometry());
settings.setValue("recentFiles", MainWindow::recentFiles);
settings.setValue("currentScheme", MainWindow::currentScheme);
//settings.setValue("lineNumbers", MainWindow::lineNumbers);
//settings.setValue("syntax", MainWindow::syntax);
//settings.setValue("lineHighlight", MainWindow::lineHighlight);
//settings.setValue("hidescratch", MainWindow::hidescratch);
//settings.setValue("linewrap", MainWindow::linewrap);

}

void MainWindow::readSettings()
{
QSettings settings("FundComp", "NotePad2");
QRect rect = settings.value("geometry",
QRect(200, 200, 400, 400)).toRect();
move(rect.topLeft());
resize(rect.size());
MainWindow::recentFiles = settings.value("recentFiles").toStringList();
MainWindow::currentScheme = settings.value("currentScheme").toInt();
//MainWindow::lineNumbers = settings.value("lineNumbers").toInt();
//MainWindow::syntax = settings.value("syntax").toInt();
//MainWindow::lineHighlight = settings.value("lineHighlight").toInt();
//MainWindow::hidescratch = settings.value("hidescratch").toInt();
//MainWindow::linewrap = settings.value("linewrap").toInt();


foreach (QWidget *win, QApplication::topLevelWidgets())
{
if (MainWindow *mainWin = qobject_cast<MainWindow *>(win))
mainWin->updateRecentFileActions();
}
}
