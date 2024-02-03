#include "lippointgraphicsitem.h"

LIPPointGraphicsItem::LIPPointGraphicsItem()
    : LIPGraphicsItem()
{

}

LIPPointGraphicsItem::~LIPPointGraphicsItem()
{
    delete p;
}

QRectF LIPPointGraphicsItem::boundingRect() const
{
    return QRectF(p->x()-(mPointSize/2)/mSceneScale, p->y()-(mPointSize/2)/mSceneScale,(mPointSize/2)/mSceneScale,(mPointSize/2)/mSceneScale);
}

void LIPPointGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen = mStyle->getPen();
    pen.setWidthF(pen.widthF()/mSceneScale);
    painter->setPen(pen);
    painter->setBrush(mStyle->getBrush());
    double pointSize=LIPVectorStyle::MMToPixel(mStyle->getPointSize());
    mPointSize=pointSize;
    painter->drawEllipse(QPointF(p->x(), p->y()), (pointSize/2)/mSceneScale, (pointSize/2)/mSceneScale);
}

void LIPPointGraphicsItem::setPoint(LIPPoint *point)
{
    p=point;
}

//void LIPPointGraphicsItem::setPen(QPen pen)
//{
//    mPen=pen;
//}

//void LIPPointGraphicsItem::setBrush(QBrush brush)
//{
//    mBrush=brush;
//}

//void LIPPointGraphicsItem::setVectorStyle(LIPVectorStyle *style)
//{
//    mStyle=style;
//    setPen(mStyle->getPen());
//    setBrush(mStyle->getBrush());
//    double pointSize=mStyle->getPointSize();
//    bRect.setRect(p->x()-LIPVectorStyle::MMToPixel(pointSize/2),
//                  p->y()-LIPVectorStyle::MMToPixel(pointSize/2),
//                  LIPVectorStyle::MMToPixel(pointSize),
//                  LIPVectorStyle::MMToPixel(pointSize));
//}



