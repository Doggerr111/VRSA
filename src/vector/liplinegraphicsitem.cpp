#include "liplinegraphicsitem.h"

LIPLineGraphicsItem::LIPLineGraphicsItem()
    :   LIPGraphicsItem()
{
    mPen.setWidthF(LIPVectorStyle::MMToPixel(1)/mSceneScale);

}

LIPLineGraphicsItem::~LIPLineGraphicsItem()
{

}

void LIPLineGraphicsItem::setPoints(QVector<LIPPoint *> points)
{
    for (int i=0; i<points.size(); i++)
    {
        QPointF p=QPointF(points.at(i)->x(),points.at(i)->y());
        vect.append(p);
    }
    calculateBoundingRect();
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
    return(bRect);
}

void LIPLineGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen = mStyle->getPen();
    pen.setWidthF(LIPVectorStyle::MMToPixel(pen.widthF())/mSceneScale);
    painter->setPen(pen);
    //mPen.setWidthF((mPen.widthF())/mSceneScale);
    //painter->setBrush(mBrush);
    painter->drawPolyline(vect);

}


QPainterPath LIPLineGraphicsItem::shape() const
{
    return QPainterPath();
}
