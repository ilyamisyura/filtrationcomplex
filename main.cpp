#include "mainwindow.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{

    QTime t = QTime::currentTime();
    qsrand((uint)t.msec());

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
