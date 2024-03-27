#ifndef TESTLIPVECTORCONVERTOR_H
#define TESTLIPVECTORCONVERTOR_H


#include "lipvectorconvertor.h"
#include <QTest>
class testLIPVectorConvertor : public QObject
{
    Q_OBJECT
public:
    testLIPVectorConvertor();
    ~testLIPVectorConvertor();
private slots:
    void test_vectorPointtoGeosLine();
    void test_vectorPointstoGeosPoints();
    void test_vectorPointstoGeosPolygon();




};

#endif // TESTLIPVECTORCONVERTOR_H
