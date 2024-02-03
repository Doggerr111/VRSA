#include "liptemporarygraphicsitem.h"

LIPTemporaryGraphicsItem::LIPTemporaryGraphicsItem(LIPGeometryType type)
    : geomType{type}
{

}

QRectF LIPTemporaryGraphicsItem::boundingRect() const
{
    return QRectF();
}

void LIPTemporaryGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch (geomType)
    {
        case LIPGeometryType::LIPPoint: //будет использоваться, например, при редактировании слоев (вершины)
            //painter->drawpo
            break;
        case LIPGeometryType::LIPLineString: //

            break;
        case LIPGeometryType::LIPPolygon:
            break;
    }
}
