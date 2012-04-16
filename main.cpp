#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QFont>

QStringList MainWindow::recentFiles;
    int MainWindow::currentScheme;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font("Lucida", 10, 0, false);
    MainWindow * mainWin = new MainWindow;
    a.setFont(font, "QPlainTextEdit");
   // mainWin->resize(640, 640);
    mainWin->show();
    
    return a.exec();
}
