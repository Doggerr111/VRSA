#include "lippointgraphicsitem.h"

LIPPointGraphicsItem::LIPPointGraphicsItem()
    : LIPGraphicsItem()
{

}

LIPPointGraphicsItem::~LIPPointGraphicsItem()
{
    //delete p;

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
    QBrush brush = mStyle->getBrush();
    brush.setTransform(QTransform(painter->worldTransform().inverted())); //обязательно для корректного применения стилей кисти
    painter->setBrush(brush);

    double pointSize=LIPVectorStyle::MMToPixel(mStyle->getPointSize());
    mPointSize=pointSize;
    //mStyle->setPointType(Rhombus);

    switch (mStyle->getPointType())
    {
    case Circle:
        painter->drawEllipse(QPointF(p->x(), p->y()), (pointSize/2)/mSceneScale, (pointSize/2)/mSceneScale);
        break;
    case Square:
        painter->drawRect(QRectF(p->x()-(pointSize/2)/mSceneScale, p->y()-(pointSize/2)/mSceneScale,
                                 (pointSize)/mSceneScale, (pointSize)/mSceneScale));
        break;
    case Triangle:
    {
//        QPointF point1(p->x()+pointSize/2,p->y()-(pointSize)/mSceneScale);
//        QPointF point2(p->x()-(pointSize/2)/mSceneScale, p->y());
//        QPointF point3(p->x()-(pointSize)/mSceneScale,p->y()-(pointSize)/mSceneScale);
        QPointF point1(p->x()-(pointSize/2)/mSceneScale,p->y()-(pointSize/2)/mSceneScale);
        QPointF point2(p->x(), p->y()+(pointSize/2)/mSceneScale);
        QPointF point3(p->x()+(pointSize/2)/mSceneScale,p->y()-(pointSize/2)/mSceneScale);
        QVector<QPointF> points{point1,point2,point3};
        QPolygonF pol(std::move(points));
        painter->drawPolygon(pol);
        break;
    }
    case InvertedTriangle:
    {
        QPointF point1(p->x()+(pointSize/2)/mSceneScale,p->y()+(pointSize/2)/mSceneScale);
        QPointF point2(p->x(), p->y()-(pointSize/2)/mSceneScale);
        QPointF point3(p->x()-(pointSize/2)/mSceneScale,p->y()+(pointSize/2)/mSceneScale);
        QVector<QPointF> points{point1,point2,point3};
        QPolygonF pol(std::move(points));
        painter->drawPolygon(pol);
        break;
    }
    case Rhombus:
    {
        QPointF point1(p->x()-(pointSize/2)/mSceneScale,p->y());
        QPointF point2(p->x(), p->y()+(pointSize/2)/mSceneScale);
        QPointF point3(p->x()+(pointSize/2)/mSceneScale,p->y());
        QPointF point4(p->x(), p->y()-(pointSize/2)/mSceneScale);
        QVector<QPointF> points{point1,point2,point3, point4};
        QPolygonF pol(std::move(points));
        painter->drawPolygon(pol);
        break;
    }
    case CustomImage:
    {
        painter->drawImage(QRectF(p->x()-(pointSize/2)/mSceneScale, p->y()-(pointSize/2)/mSceneScale,
                                  (pointSize)/mSceneScale, (pointSize)/mSceneScale), *mStyle->getCustomImage());
    }


}

    //painter->drawImage(QRectF(p->x(),p->y(),(50/2)/mSceneScale, (50/2)/mSceneScale),
    //                   test);
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



