#ifndef TESTINIT_H
#define TESTINIT_H
#include <QObject>
#include "testlipvectorconvertor.h"
#include "testlipxyzconnection.h"
#include <QTest>
class testInit: public QObject
{
    Q_OBJECT
public:
    testInit();
    //~testInit();
};

#endif // TESTINIT_H
