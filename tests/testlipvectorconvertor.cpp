#include "testlipvectorconvertor.h"

testLIPVectorConvertor::testLIPVectorConvertor()
{

}

testLIPVectorConvertor::~testLIPVectorConvertor()
{

}

void testLIPVectorConvertor::test_vectorPointtoGeosLine()
{
    QVector<QVector<LIPPoint*>> lipPolygon;
    QVector<LIPPoint*> polyFeature1;
    QVector<LIPPoint*> polyFeature2;
    QVector<LIPPoint*> polyFeature3;

    polyFeature1.append(static_cast<LIPPoint*>(new QPointF(1,1)));
    polyFeature1.append(static_cast<LIPPoint*>(new QPointF(2,2)));
    polyFeature1.append(static_cast<LIPPoint*>(new QPointF(3,3)));

    polyFeature2.append(static_cast<LIPPoint*>(new QPointF(4,4)));
    polyFeature2.append(static_cast<LIPPoint*>(new QPointF(5,5)));
    polyFeature2.append(static_cast<LIPPoint*>(new QPointF(6,6)));

    polyFeature3.append(static_cast<LIPPoint*>(new QPointF(7,7)));
    polyFeature3.append(static_cast<LIPPoint*>(new QPointF(8,8)));
    polyFeature3.append(static_cast<LIPPoint*>(new QPointF(9,9)));

    lipPolygon.append(polyFeature1);
    lipPolygon.append(polyFeature2);
    lipPolygon.append(polyFeature3);
    //получаем результаты
    auto geosLine = LIPVectorConvertor::vectorPointstoGeosLine(lipPolygon);
    QCOMPARE(geosLine->getNumGeometries(), 3);
    for (std::size_t i=0; i<geosLine->getNumGeometries(); i++)
    {
        auto line=geosLine->getGeometryN(i);

        if (i==0)
        {
            QCOMPARE(line->getCoordinates()->size(), 3);
            QCOMPARE(line->getCoordinates()->getAt(0).x,1);
            QCOMPARE(line->getCoordinates()->getAt(0).y,1);
            QCOMPARE(line->getCoordinates()->getAt(1).x,2);
            QCOMPARE(line->getCoordinates()->getAt(1).y,2);
            QCOMPARE(line->getCoordinates()->getAt(2).x,3);
            QCOMPARE(line->getCoordinates()->getAt(2).y,3);

        }

        if (i==1)
        {
            QCOMPARE(line->getCoordinates()->size(), 3);
            QCOMPARE(line->getCoordinates()->getAt(0).x,4);
            QCOMPARE(line->getCoordinates()->getAt(0).y,4);
            QCOMPARE(line->getCoordinates()->getAt(1).x,5);
            QCOMPARE(line->getCoordinates()->getAt(1).y,5);
            QCOMPARE(line->getCoordinates()->getAt(2).x,6);
            QCOMPARE(line->getCoordinates()->getAt(2).y,6);
        }

        if (i==2)
        {
            QCOMPARE(line->getCoordinates()->size(), 3);
            QCOMPARE(line->getCoordinates()->getAt(0).x,7);
            QCOMPARE(line->getCoordinates()->getAt(0).y,7);
            QCOMPARE(line->getCoordinates()->getAt(1).x,8);
            QCOMPARE(line->getCoordinates()->getAt(1).y,8);
            QCOMPARE(line->getCoordinates()->getAt(2).x,9);
            QCOMPARE(line->getCoordinates()->getAt(2).y,9);
        }
    }

}

void testLIPVectorConvertor::test_vectorPointstoGeosPoints()
{
    QVector<LIPPoint*> lipPoints;


    lipPoints.append(static_cast<LIPPoint*>(new QPointF(1,1)));
    lipPoints.append(static_cast<LIPPoint*>(new QPointF(2,2)));
    lipPoints.append(static_cast<LIPPoint*>(new QPointF(3,3)));
    lipPoints.append(static_cast<LIPPoint*>(new QPointF(4,4)));
    lipPoints.append(static_cast<LIPPoint*>(new QPointF(5,5)));
    lipPoints.append(static_cast<LIPPoint*>(new QPointF(6,6)));
    //получаем результаты
    auto geosPoints = LIPVectorConvertor::vectorPointstoGeosPoints(lipPoints);
    QCOMPARE(geosPoints->getNumGeometries(), 6);

    auto point1=geosPoints->getGeometryN(0);
    QCOMPARE(point1->getCoordinate()->x,1);
    QCOMPARE(point1->getCoordinate()->y,1);

    auto point2=geosPoints->getGeometryN(1);
    QCOMPARE(point2->getCoordinate()->x,2);
    QCOMPARE(point2->getCoordinate()->y,2);

    auto point3=geosPoints->getGeometryN(2);
    QCOMPARE(point3->getCoordinate()->x,3);
    QCOMPARE(point3->getCoordinate()->y,3);

    auto point4=geosPoints->getGeometryN(3);
    QCOMPARE(point4->getCoordinate()->x,4);
    QCOMPARE(point4->getCoordinate()->y,4);

    auto point5=geosPoints->getGeometryN(4);
    QCOMPARE(point5->getCoordinate()->x,5);
    QCOMPARE(point5->getCoordinate()->y,5);

    auto point6=geosPoints->getGeometryN(5);
    QCOMPARE(point6->getCoordinate()->x,6);
    QCOMPARE(point6->getCoordinate()->y,6);

}

void testLIPVectorConvertor::test_vectorPointstoGeosPolygon()
{
    //формируем исходные данные
   QVector<QVector<LIPPoint*>> lipPolygon;
   QVector<LIPPoint*> polyFeature1;
   QVector<LIPPoint*> polyFeature2;
   QVector<LIPPoint*> polyFeature3;

   polyFeature1.append(static_cast<LIPPoint*>(new QPointF(1,1)));
   polyFeature1.append(static_cast<LIPPoint*>(new QPointF(2,2)));
   polyFeature1.append(static_cast<LIPPoint*>(new QPointF(3,3)));

   polyFeature2.append(static_cast<LIPPoint*>(new QPointF(4,4)));
   polyFeature2.append(static_cast<LIPPoint*>(new QPointF(5,5)));
   polyFeature2.append(static_cast<LIPPoint*>(new QPointF(6,6)));

   polyFeature3.append(static_cast<LIPPoint*>(new QPointF(7,7)));
   polyFeature3.append(static_cast<LIPPoint*>(new QPointF(8,8)));
   polyFeature3.append(static_cast<LIPPoint*>(new QPointF(9,9)));

   lipPolygon.append(polyFeature1);
   lipPolygon.append(polyFeature2);
   lipPolygon.append(polyFeature3);
   //получаем результаты
   auto geosPolygon = LIPVectorConvertor::vectorPointstoGeosPolygon(lipPolygon);
   QCOMPARE(geosPolygon->getNumGeometries(), 3);
   for (std::size_t i=0; i<geosPolygon->getNumGeometries(); i++)
   {
       auto poly=geosPolygon->getGeometryN(i);

       if (i==0)
       {
           QCOMPARE(poly->getCoordinates()->size(), 4);
           QCOMPARE(poly->getCoordinates()->getAt(0).x,1);
           QCOMPARE(poly->getCoordinates()->getAt(0).y,1);
           QCOMPARE(poly->getCoordinates()->getAt(1).x,2);
           QCOMPARE(poly->getCoordinates()->getAt(1).y,2);
           QCOMPARE(poly->getCoordinates()->getAt(2).x,3);
           QCOMPARE(poly->getCoordinates()->getAt(2).y,3);
           //проверяем, что полигон "закрыт"
           QCOMPARE(poly->getCoordinates()->getAt(3).x,1);
           QCOMPARE(poly->getCoordinates()->getAt(3).y,1);
       }

       if (i==1)
       {
           QCOMPARE(poly->getCoordinates()->size(), 4);
           QCOMPARE(poly->getCoordinates()->getAt(0).x,4);
           QCOMPARE(poly->getCoordinates()->getAt(0).y,4);
           QCOMPARE(poly->getCoordinates()->getAt(1).x,5);
           QCOMPARE(poly->getCoordinates()->getAt(1).y,5);
           QCOMPARE(poly->getCoordinates()->getAt(2).x,6);
           QCOMPARE(poly->getCoordinates()->getAt(2).y,6);
       }

       if (i==2)
       {
           QCOMPARE(poly->getCoordinates()->size(), 4);
           QCOMPARE(poly->getCoordinates()->getAt(0).x,7);
           QCOMPARE(poly->getCoordinates()->getAt(0).y,7);
           QCOMPARE(poly->getCoordinates()->getAt(1).x,8);
           QCOMPARE(poly->getCoordinates()->getAt(1).y,8);
           QCOMPARE(poly->getCoordinates()->getAt(2).x,9);
           QCOMPARE(poly->getCoordinates()->getAt(2).y,9);
       }
   }
}
