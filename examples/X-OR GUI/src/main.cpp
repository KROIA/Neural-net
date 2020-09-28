// Autor        Alex Krieg
// Datum        28.09.2020
// Version      00.01.00

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
