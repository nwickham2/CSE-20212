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

//#include "linenumberarea.h"
//#include "codeeditor.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
textarea = new QPlainTextEdit;
setCentralWidget(textarea);
setAttribute(Qt::WA_DeleteOnClose);

createActions();
createMenus();
createToolBars();
readSettings();
setCurrentFile("");

statusBar()->showMessage(tr("Welcome to NotePad2"));

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


//createContextMenu();
//createStatusBar();

/*splitter2 = new QSplitter;

model = new QFileSystemModel;
model->setRootPath(QDir::currentPath());
tree = new QTreeView(splitter);
    tree->setModel(model);//QDir::currentPath());
    tree->setRootIndex(model->index(QDir::currentPath()));
         files = new QDockWidget(tr("Files"), this);
         addDockWidget(Qt::LeftDockWidgetArea, files);
         files->setWidget(splitter);*/
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
    lineNumberAction->setStatusTip(tr("Highlight the document with C++ syntax"));
    connect(lineNumberAction, SIGNAL(toggled(bool)), this, SLOT(doLineNumbers(bool)));

  //connect(tree, SIGNAL(clicked(QModelIndex)), this, SLOT(openTree()));
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

    optionsMenu = menuBar()->addMenu(tr("&Options"));
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
        highlighter = new Highlighter(textarea->document());
    else
        delete highlighter;
}

void MainWindow::doLineNumbers(bool x)
{
    QTabWidget::TabPosition order;
    order = tabPosition(Qt::BottomDockWidgetArea);
    QString temp, temp1, temp2;
    QPlainTextEdit *holder = new QPlainTextEdit;
    setCentralWidget(holder);
    temp = textarea->toPlainText();
    temp1 = scratcharea1->toPlainText();
    temp2 = scratcharea2->toPlainText();
    if(x == true)
    {
        delete textarea;
        textarea = new CodeEditor(temp);
    }
    else
    {
        delete textarea;
        textarea = new QPlainTextEdit(temp);
    }
    setCentralWidget(textarea);
    createActions();

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
textarea->clear();
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            textarea->appendPlainText(line);
        }
    setCurrentFile(fileName);
    statusBar() -> showMessage(tr("File loaded"), 2000);
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
}

void MainWindow::readSettings()
{
QSettings settings("FundComp", "NotePad2");
QRect rect = settings.value("geometry",
QRect(200, 200, 400, 400)).toRect();
move(rect.topLeft());
resize(rect.size());
MainWindow::recentFiles = settings.value("recentFiles").toStringList();
foreach (QWidget *win, QApplication::topLevelWidgets())
{
if (MainWindow *mainWin = qobject_cast<MainWindow *>(win))
mainWin->updateRecentFileActions();
}
}

/*void MainWindow::openTree()
{
    cout << "Hello" << endl;
    QModelIndex index = tree->currentIndex();
     QString text = model->data(index, Qt::DisplayRole).toString();
     loadFile(text);

}*/
