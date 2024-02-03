#ifndef LIPPOINTGRAPHICSITEM_H
#define LIPPOINTGRAPHICSITEM_H
#include <QGraphicsItem>
#include <QPainter>
#include "vector/lippoint.h"
#include "lipvectorstyle.h"
#include "vector/lipgraphicsitem.h"
class LIPPointGraphicsItem: public LIPGraphicsItem
{
public:
    LIPPointGraphicsItem();
    ~LIPPointGraphicsItem();
    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setPoint(LIPPoint *point);
    void setPen(QPen pen);
    void setBrush(QBrush brush);
    //void setVectorStyle(LIPVectorStyle*);
private:
    LIPPoint *p;

    QRectF bRect;

    // QGraphicsItem interface
public:
    //QPainterPath shape() const;
};

#endif // LIPPOINTGRAPHICSITEM_H
