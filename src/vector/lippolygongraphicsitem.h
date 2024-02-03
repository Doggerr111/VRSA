#ifndef LIPPOLYGONGRAPHICSITEM_H
#define LIPPOLYGONGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPen>
#include <QPainter>
#include <vector/lippoint.h>
#include <vector/lipgraphicsitem.h>
class LIPPolygonGraphicsItem : public LIPGraphicsItem
{
public:
    LIPPolygonGraphicsItem();
    ~LIPPolygonGraphicsItem() = default;
    void setPoints(QVector<LIPPoint*> points);
private:
    QVector<QPointF> vect;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // LIPPOLYGONGRAPHICSITEM_H
