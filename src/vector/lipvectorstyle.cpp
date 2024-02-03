#include "lipvectorstyle.h"

LIPVectorStyle::LIPVectorStyle()
{

}

LIPVectorStyle::~LIPVectorStyle()
{

}

void LIPVectorStyle::setStyleUnit(LIPStyleUnit unit)
{
    mStyleUnit=unit;
}

void LIPVectorStyle::setPen(QPen pen)
{
    mPen=pen;
}

void LIPVectorStyle::setWidthMM(double width)
{
     mPen.setWidthF(MMToPixel(width)/mSceneScaleFact);
}

void LIPVectorStyle::setWidth(double width)
{

}

void LIPVectorStyle::setPenColor(QColor penCol)
{
    mPen.setColor(penCol);
}

void LIPVectorStyle::setPointSize(double sizeF)
{
    mPointSize=sizeF;
}

void LIPVectorStyle::setBrush(QBrush brush)
{
    mBrush=brush;
}

void LIPVectorStyle::setBrushColor(QColor brCol)
{
    mBrush.setColor(brCol);
}

void LIPVectorStyle::setGeomType(LIPGeometryType type)
{
    mGeomType=type;
}

QPen LIPVectorStyle::getPen()
{
    return mPen;
}

QBrush LIPVectorStyle::getBrush()
{
    return mBrush;
}

double LIPVectorStyle::getPointSize()
{
    return mPointSize;
}

LIPGeometryType LIPVectorStyle::GetGeomType()
{
    return mGeomType;
}

double LIPVectorStyle::pixelToMM(double pix)
{
    return 0.0;
}

double LIPVectorStyle::MMToPixel(double mm)
{
    const double dots_per_millimeter = (qApp->primaryScreen()->physicalDotsPerInch() / 25.40);
    return mm*dots_per_millimeter;
}

LIPVectorStyle* LIPVectorStyle::createDefaultVectorStyle(LIPGeometryType type)
{
    QPen pen;
    LIPVectorStyle* st = new LIPVectorStyle;
    switch (type)
    {
    case LIPGeometryType::LIPPoint:
    {
        pen.setWidthF(0);
        QBrush brush;
        brush.setColor(QColor::fromRgb(QRandomGenerator::global()->bounded(0, 256),
                                       QRandomGenerator::global()->bounded(0, 256),
                                       QRandomGenerator::global()->bounded(0, 256)));
        brush.setStyle(Qt::SolidPattern);
        st->setBrush(brush);
        st->setPen(pen);
        st->setPointSize(1);
        st->setGeomType(type);
        return st;
    }
    case LIPGeometryType::LIPLineString:
    {
        pen.setWidthF(LIPVectorStyle::MMToPixel(0.1));
        pen.setColor(QColor::fromRgb(QRandomGenerator::global()->bounded(0, 256),
                                       QRandomGenerator::global()->bounded(0, 256),
                                       QRandomGenerator::global()->bounded(0, 256)));
        st->setGeomType(type);
        st->setPen(pen);
        return st;
    }
    case LIPGeometryType::LIPPolygon:
    {
        QBrush brush;
        pen.setWidthF(0);
        brush.setColor(QColor::fromRgb(QRandomGenerator::global()->bounded(0, 256),
                                       QRandomGenerator::global()->bounded(0, 256),
                                       QRandomGenerator::global()->bounded(0, 256)));
        brush.setStyle(Qt::SolidPattern);
        st->setBrush(brush);
        st->setPen(pen);
        st->setPointSize(1);
        st->setGeomType(type);
        return st;
    }
    }
}

//LIPVectorStyle *LIPVectorStyle::selectedStyle(LIPVectorStyle* style)
//{
//    LIPVectorStyle* st = style;
//    st->setBrushColor(Qt::red);
//    st->setPenColor(Qt::red);
//    return st;

//}
