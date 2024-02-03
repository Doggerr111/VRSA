#include "lippoint.h"

LIPPoint::LIPPoint()
{

}

void LIPPoint::draw(QPainter *painter)
{
    rect=QRectF(x(),y(),0.1,0.1);
    painter->drawEllipse(rect);

}
