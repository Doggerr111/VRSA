#ifndef TESTLIPXYZCONNECTION_H
#define TESTLIPXYZCONNECTION_H

#include <QTest>
#include "lipxyzconnection.h"
class testLIPXYZConnection: public QObject
{
    Q_OBJECT
public:
    testLIPXYZConnection();
    ~testLIPXYZConnection();
private slots:
    void testMetersToTile();
    void testCheckMinMaxXY();
};

#endif // TESTLIPXYZCONNECTION_H
