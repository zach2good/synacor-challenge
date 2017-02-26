#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    static std::string location = argv[0];

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
