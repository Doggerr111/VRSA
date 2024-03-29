#include "lipgraphicsitem.h"
#include "qmessagebox.h"
LIPGraphicsItem::LIPGraphicsItem()
    : mSceneScale{1.0},
      mIndex{-1},
      mIsSelected{false}
{
    mPen.setWidthF(0);
    mBrush.setColor(Qt::blue);
    mBrush.setStyle(Qt::SolidPattern);
    mStyle=new LIPVectorStyle;
    mStyle->setPen(mPen);
    mStyle->setBrush(mBrush);
    mStyle->setPointSize(1);
}

LIPGraphicsItem::~LIPGraphicsItem()
{
    //mStyle=nullptr;
    //delete mStyle;
}

void LIPGraphicsItem::setPen(QPen pen)
{
    mPen=pen;
}

void LIPGraphicsItem::setBrush(QBrush brush)
{
    mBrush=brush;
}

void LIPGraphicsItem::setVectorStyle(LIPVectorStyle *style)
{
    mStyle=style;
    setPen(mStyle->getPen());
    setBrush(mStyle->getBrush());

}

void LIPGraphicsItem::setScaleFactor(double f)
{
    mSceneScale=f;
}

void LIPGraphicsItem::select()
{
    mIsSelected=true;
    update();
}

void LIPGraphicsItem::deselect()
{
    mIsSelected=false;
    update();
}

bool LIPGraphicsItem::isSelected()
{
    return mIsSelected;

}

void LIPGraphicsItem::setIndex(int ind)
{
    mIndex=ind;
}

int LIPGraphicsItem::getIndex()
{
    return mIndex;
}


QRectF LIPGraphicsItem::boundingRect() const
{
}

void LIPGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void LIPGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    emit clicked(mIndex);

}

//void LIPGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    emit clicked(mIndex);
//    QGraphicsItem::mousePressEvent(event);
//}
