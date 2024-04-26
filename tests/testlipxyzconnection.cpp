#include "testlipxyzconnection.h"

testLIPXYZConnection::testLIPXYZConnection()
{

}

testLIPXYZConnection::~testLIPXYZConnection()
{

}

void testLIPXYZConnection::testMetersToTile()
{
    LIPXYZConnection* connection = new LIPXYZConnection("",1,20);
    //test for 1 zoom level
    connection->setZoomLevel(1);
    auto topLeftPoint = connection->metersToTile(QPointF(-8570015.18, 11487467.16));
    QCOMPARE(topLeftPoint.x(), 0);
    QCOMPARE(topLeftPoint.y(), 1);

    QPoint topRightPoint = connection->metersToTile(QPointF(10231615.55, 8185292.44));
    QCOMPARE(topRightPoint.x(), 1);
    QCOMPARE(topRightPoint.y(), 1);

    QPoint bottomLeftPoint = connection->metersToTile(QPointF(-13054130.19, -13124693.05));
    QCOMPARE(bottomLeftPoint.x(), 0);
    QCOMPARE(bottomLeftPoint.y(), 0);

    QPoint bottomRightPoint = connection->metersToTile(QPointF(14324261.77, -12771878.72));
    QCOMPARE(bottomRightPoint.x(), 1);
    QCOMPARE(bottomRightPoint.y(), 0);
    delete connection;

}

void testLIPXYZConnection::testCheckMinMaxXY()
{
    LIPXYZConnection* connection = new LIPXYZConnection("",0,20);
    connection->setZoomLevel(1);
    QPoint minXY(-1,-1);
    QPoint maxXY(2,2);
    connection->checkMinMaxXY(minXY, maxXY);
    QCOMPARE(minXY.x(), 0);
    QCOMPARE(minXY.y(), 0);

    QCOMPARE(maxXY.x(), 1);
    QCOMPARE(maxXY.y(), 1);
}
