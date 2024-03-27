#include "lippolygongraphicsitem.h"

LIPPolygonGraphicsItem::LIPPolygonGraphicsItem()
    :   LIPGraphicsItem()
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

LIPPolygonGraphicsItem::~LIPPolygonGraphicsItem()
{

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
    //return QRectF();
    return(QPolygonF(vect).boundingRect());
}

void LIPPolygonGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen = mStyle->getPen();
    pen.setWidthF(LIPVectorStyle::MMToPixel(pen.widthF())/mSceneScale);
    painter->setPen(pen);
    QBrush brush = mStyle->getBrush();
    //brush.setStyle(Qt::Dense5Pattern);
    brush.setTransform(QTransform(painter->worldTransform().inverted())); //обязательно для корректного применения стилей кисти
    painter->setBrush(brush);
    painter->drawPolygon(vect);
}
