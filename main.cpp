#include <QtGui/QApplication>
/*
Nate Wickham
Dylan Zaragoza
Byron Zaragoza

main.cpp
5/2/2012

main actually runs the applicaiton with the mainwindow.  It sets the font, the splashscreen, and the icon.
*/

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
    //splash->setPixmap(QPixmap("C:/Users/Byron/CSE-20212/Images/Splash.png"));
    splash->show();

    a.setWindowIcon(QIcon("C:/Users/Nate/Desktop/Qtstuff/texteditor/Icon.png"));
    // a.setWindowIcon(QIcon("C:/Users/Byron/CSE-20212/Images/Icon.png"));
    a.processEvents();
    sleep(3);
    QFont font("Lucida", 10, 0, false);
    MainWindow * mainWin = new MainWindow;
    a.setFont(font, "QPlainTextEdit");
    mainWin->show();

    splash->finish(mainWin);

    return a.exec();
}
