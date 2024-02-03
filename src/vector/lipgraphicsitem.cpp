#include "lipgraphicsitem.h"
#include "qmessagebox.h"
LIPGraphicsItem::LIPGraphicsItem()
    : mSceneScale{1.0}
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
    mStyle=nullptr;
    delete mStyle;
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
    mStyle->setPenColor(Qt::red);
    mStyle->setBrushColor(Qt::red);
}
