#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    MainWindow w;
    w.show();
    GDALAllRegister();
    return a.exec();
}
