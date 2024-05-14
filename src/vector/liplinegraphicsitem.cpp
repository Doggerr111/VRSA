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
    //return(QPolygonF(vect).boundingRect());

    QPolygonF polyLine = vect; // Ваши координаты точек линии
        // Находим минимальное и максимальное значение x и y
        qreal minX = polyLine.first().x();
        qreal maxX = polyLine.first().x();
        qreal minY = polyLine.first().y();
        qreal maxY = polyLine.first().y();

        for (const QPointF& point : polyLine) {
            minX = qMin(minX, point.x());
            maxX = qMax(maxX, point.x());
            minY = qMin(minY, point.y());
            maxY = qMax(maxY, point.y());
        }

        qreal penWidth =LIPVectorStyle::MMToPixel(mStyle->getPen().widthF()/mSceneScale) ; // Ширина линии, которую используете
        qreal halfPenWidth = penWidth / 2.0; // Половина толщины линии

        // Расширяем boundingRect() на половину толщины линии во всех направлениях
        return QRectF(minX - halfPenWidth, minY - halfPenWidth, maxX - minX + penWidth, maxY - minY + penWidth);
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
        pen.setCapStyle(Qt::SquareCap);
        pen.setWidthF(LIPVectorStyle::MMToPixel(pen.widthF())/mSceneScale);
        painter->setPen(pen);
    }
    painter->drawPolyline(vect);


}


//QPainterPath LIPLineGraphicsItem::shape() const
//{
//    return QPainterPath();
//}
