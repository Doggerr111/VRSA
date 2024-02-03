#ifndef LIPLINEGRAPHICSITEM_H
#define LIPLINEGRAPHICSITEM_H

#include <QGraphicsItem>
#include <vector/lippoint.h>
#include <vector/lipgraphicsitem.h>
class LIPLineGraphicsItem : public LIPGraphicsItem
{
public:
    LIPLineGraphicsItem();
    ~LIPLineGraphicsItem();
    void setPoints(QVector<LIPPoint*> points);
private:
    QVector<QPointF> vect;
    void calculateBoundingRect();
    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF bRect;

    // QGraphicsItem interface
public:
    QPainterPath shape() const;
};

#endif // LIPLINEGRAPHICSITEM_H
