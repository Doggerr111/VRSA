#include "liplinegraphicsitem.h"
#include <QDebug>
LIPLineGraphicsItem::LIPLineGraphicsItem()
    :   LIPGraphicsItem()
{
    mPen.setWidthF(LIPVectorStyle::MMToPixel(1)/mSceneScale);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

LIPLineGraphicsItem::~LIPLineGraphicsItem()
{

}

void LIPLineGraphicsItem::setPoints(QVector<LIPPoint *> points)
{
    vect.clear();

    for (int i=0; i<points.size(); i++)
    {

        QPointF p=QPointF(points.at(i)->x(),points.at(i)->y());
        vect.append(p);

    }
    //calculateBoundingRect();
    prepareGeometryChange();
    update();

    //vect=points;
}

void LIPLineGraphicsItem::calculateBoundingRect()
{
    for (const QPointF& point : vect) {
        if (bRect.isNull()) {
            // Если исходный bounding rectangle пуст, установить первую точку
            bRect = QRectF(point, QSizeF(0, 0));
        } else {
            // Объединить текущий bounding rectangle с текущей точкой
            bRect = bRect.united(QRectF(point, QSizeF(0, 0)));
        }
    }

}

QRectF LIPLineGraphicsItem::boundingRect() const
{
    //return QRectF();
    return(QPolygonF(vect).boundingRect());
}

void LIPLineGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    QPen pen;
////    QVector<QPointF> p;
////    for(auto point:vect)
////    {
////        p.append(QPointF(point.x()-0.01, point.y()-0.01));
////    }
//    pen.setColor(Qt::black);
//    pen.setWidthF(LIPVectorStyle::MMToPixel(pen.widthF()+3)/mSceneScale);
//    painter->setPen(pen);
//    painter->drawPolyline(vect);

    QPen pen = mStyle->getPen();
    //painter->setRenderHint(QPainter::Antialiasing, false);
    pen.setWidthF(LIPVectorStyle::MMToPixel(pen.widthF())/mSceneScale);
    //qDebug()<<"pen w";
    //qDebug()<<QString::number(pen.widthF(), 'f', 20);
    painter->setPen(pen);
    //mPen.setWidthF((mPen.widthF())/mSceneScale);
    //painter->setBrush(mBrush);

//    for (int i=1; i< vect.size(); i++)
//    {
//        painter->drawLine(vect[i-1], vect[i]);
//    }
    painter->drawPolyline(vect);

    //painter->drawPath(path);
   //painter->drawPolygon(vect);
    //painter->drawLines(vect);






}


//QPainterPath LIPLineGraphicsItem::shape() const
//{
//    return QPainterPath();
//}
