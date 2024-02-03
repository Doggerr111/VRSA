#include "lippolygongraphicsitem.h"

LIPPolygonGraphicsItem::LIPPolygonGraphicsItem()
    :   LIPGraphicsItem()
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

void LIPPolygonGraphicsItem::setPoints(QVector<LIPPoint *> points)
{
    for (int i=0; i<points.size(); i++)
    {
        QPointF p=QPointF(points.at(i)->x(),points.at(i)->y());
        vect.append(p);
    }
}

QRectF LIPPolygonGraphicsItem::boundingRect() const
{
    return(QPolygonF(vect).boundingRect());
}

void LIPPolygonGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen = mStyle->getPen();
    pen.setWidthF(pen.widthF()/mSceneScale);
    painter->setPen(pen);
    painter->setBrush(mStyle->getBrush());
    painter->drawPolygon(vect);
}
