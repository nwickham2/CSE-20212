#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QFont>
#include <QSplashScreen>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "highlighter.h"

QStringList MainWindow::recentFiles;
    int MainWindow::currentScheme;

    void sleep( time_t delay )
    {
        time_t timer0, timer1;
        time( &timer0 );
        do {
        time( &timer1 );
        } while (( timer1 - timer0 ) < delay );
    }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
        splash->setPixmap(QPixmap("C:/Users/Nate/Desktop/Qtstuff/texteditor/Splash.png"));
        splash->show();

        a.processEvents();
        sleep(3);
    QFont font("Lucida", 10, 0, false);
    MainWindow * mainWin = new MainWindow;
    a.setFont(font, "QPlainTextEdit");
    mainWin->show();

    return a.exec();
}
