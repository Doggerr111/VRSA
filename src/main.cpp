#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QTest>
#include "testinit.h"
#define TEST_MODE
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    #ifdef TEST_MODE
    testInit test;
    QTest::qExec(&test);
    #endif
    //QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    MainWindow w;
    w.show();
    GDALAllRegister();
    return a.exec();
}
