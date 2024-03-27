#ifndef LIPTEST_TEST_H
#define LIPTEST_TEST_H
#include <QTest>
//#include "vector/lipvectorconvertor.h"

class LIPTEST_TEST : public QObject
{
    Q_OBJECT
public:
    LIPTEST_TEST();
    ~LIPTEST_TEST();

private slots:
    void test_vectorPointstoGeosPolygon();
    void test_vectorPointstoGeosPoints();

};

#endif // LIPTEST_TEST_H
