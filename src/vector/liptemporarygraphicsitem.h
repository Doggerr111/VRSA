#ifndef LIPTEMPORARYGRAPHICSITEM_H
#define LIPTEMPORARYGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QObject>
#include "liptypes.h"
#include <QPainter>
class LIPTemporaryGraphicsItem : public QGraphicsItem
{

public:
    LIPTemporaryGraphicsItem(LIPGeometryType type);

private:
    LIPGeometryType geomType;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // LIPTEMPORARYGRAPHICSITEM_H
