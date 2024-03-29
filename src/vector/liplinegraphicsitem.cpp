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

    if (mIsSelected)
    {
        LIPVectorStyle newStyle = LIPVectorStyle::getSelectedStyle(mStyle);
        QPen pen = newStyle.getPen();
        pen.setWidthF(LIPVectorStyle::MMToPixel(pen.widthF())/mSceneScale);
        painter->setPen(pen);
    }
    else
    {
        QPen pen = mStyle->getPen();
        pen.setWidthF(LIPVectorStyle::MMToPixel(pen.widthF())/mSceneScale);
        painter->setPen(pen);
    }
    painter->drawPolyline(vect);


}


//QPainterPath LIPLineGraphicsItem::shape() const
//{
//    return QPainterPath();
//}
