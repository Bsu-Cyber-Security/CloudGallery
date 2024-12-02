#include "mainwindow.h"
#include "welcomepage.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    WelcomePage* welcomePage = new WelcomePage();
    welcomePage->show();
    return a.exec();
}
