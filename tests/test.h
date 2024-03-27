#ifndef TEST_H
#define TEST_H
#include <QTest>
//#include "vector/lipvectorconvertor.h"

//class Test : public QObject
//{
//    Q_OBJECT

//public:
//    Test();
//    ~Test();

//private slots:
//    void test_vectorPointstoGeosPolygon();

//};

//Test::Test()
//{

//}

//Test::~Test()
//{

//}

//void Test::test_vectorPointstoGeosPolygon()
//{
//    //формируем исходные данные
//    QVector<QVector<LIPPoint*>> lipPolygon;
//    QVector<LIPPoint*> polyFeature1;
//    QVector<LIPPoint*> polyFeature2;
//    QVector<LIPPoint*> polyFeature3;

//    polyFeature1.append(static_cast<LIPPoint*>(new QPointF(1,1)));
//    polyFeature1.append(static_cast<LIPPoint*>(new QPointF(2,2)));
//    polyFeature1.append(static_cast<LIPPoint*>(new QPointF(1,1)));

//    polyFeature2.append(static_cast<LIPPoint*>(new QPointF(4,4)));
//    polyFeature2.append(static_cast<LIPPoint*>(new QPointF(5,5)));
//    polyFeature2.append(static_cast<LIPPoint*>(new QPointF(4,4)));

//    polyFeature3.append(static_cast<LIPPoint*>(new QPointF(7,7)));
//    polyFeature3.append(static_cast<LIPPoint*>(new QPointF(8,8)));
//    polyFeature3.append(static_cast<LIPPoint*>(new QPointF(7,7)));

//    lipPolygon.append(polyFeature1);
//    lipPolygon.append(polyFeature2);
//    lipPolygon.append(polyFeature3);
//    //получаем результаты
//    auto geosPolygon = LIPVectorConvertor::vectorPointstoGeosPolygon(lipPolygon);
//    QCOMPARE(geosPolygon->getNumGeometries(), 3);
//    for (std::size_t i=0; i<geosPolygon->getNumGeometries(); i++)
//    {
//        auto poly=geosPolygon->getGeometryN(i);

//        if (i==0)
//        {
//            QCOMPARE(poly->getCoordinates()->size(), 3);
//            QCOMPARE(poly->getCoordinates()->getAt(0).x,1);
//            QCOMPARE(poly->getCoordinates()->getAt(0).y,1);
//            QCOMPARE(poly->getCoordinates()->getAt(1).x,2);
//            QCOMPARE(poly->getCoordinates()->getAt(1).y,2);
//            QCOMPARE(poly->getCoordinates()->getAt(2).x,1);
//            QCOMPARE(poly->getCoordinates()->getAt(2).y,1);
//        }

//        if (i==1)
//        {
//            QCOMPARE(poly->getCoordinates()->size(), 3);
//            QCOMPARE(poly->getCoordinates()->getAt(0).x,4);
//            QCOMPARE(poly->getCoordinates()->getAt(0).y,4);
//            QCOMPARE(poly->getCoordinates()->getAt(1).x,5);
//            QCOMPARE(poly->getCoordinates()->getAt(1).y,5);
//            QCOMPARE(poly->getCoordinates()->getAt(2).x,4);
//            QCOMPARE(poly->getCoordinates()->getAt(2).y,4);
//        }

//        if (i==2)
//        {
//            QCOMPARE(poly->getCoordinates()->size(), 3);
//            QCOMPARE(poly->getCoordinates()->getAt(0).x,7);
//            QCOMPARE(poly->getCoordinates()->getAt(0).y,7);
//            QCOMPARE(poly->getCoordinates()->getAt(1).x,8);
//            QCOMPARE(poly->getCoordinates()->getAt(1).y,8);
//            QCOMPARE(poly->getCoordinates()->getAt(2).x,7);
//            QCOMPARE(poly->getCoordinates()->getAt(2).y,7);
//        }
//    }


//}
#endif // TEST_H
