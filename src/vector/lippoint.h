#ifndef LIPPOINT_H
#define LIPPOINT_H

#include <QGraphicsItem>
#include <QObject>
#include <QPointF>
#include <QPainter>


class LIPPoint : public QPointF
{
public:
    LIPPoint();
    void draw(QPainter* painter);
private:
    QRectF rect;

};

#endif // LIPPOINT_H
