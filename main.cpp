#include <QtGui/QApplication>
#include "mainwindow.h"

QStringList MainWindow::recentFiles;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow * mainWin = new MainWindow;
    mainWin->resize(640, 640);
    mainWin->show();
    
    return a.exec();
}
