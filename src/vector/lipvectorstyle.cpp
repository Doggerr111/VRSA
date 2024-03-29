#include "lipvectorstyle.h"

LIPVectorStyle::LIPVectorStyle()
    : mImgSymbol{nullptr}
{

}

LIPVectorStyle::~LIPVectorStyle()
{
    delete mImgSymbol;
    mImgSymbol=nullptr;
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

void LIPVectorStyle::setPointType(PointSymbolType type)
{
    mType=type;
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

void LIPVectorStyle::setCustomImage(QImage *im)
{
    if (mImgSymbol!=nullptr)
        delete mImgSymbol;
    mImgSymbol = im;
}
QImage* LIPVectorStyle::getCustomImage()
{
    return mImgSymbol;
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

PointSymbolType LIPVectorStyle::getPointType()
{
    return mType;
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
        brush.setColor(QColor::fromRgb(QRandomGenerator::global()->bounded(0, 255),
                                       QRandomGenerator::global()->bounded(0, 255),
                                       QRandomGenerator::global()->bounded(0, 255)));
        brush.setStyle(Qt::SolidPattern);
        st->setBrush(brush);
        st->setPen(pen);
        st->setPointSize(1);
        st->setGeomType(type);
        st->setPointType(Circle);
        return st;
    }
    case LIPGeometryType::LIPLineString:
    {
        pen.setWidthF(LIPVectorStyle::MMToPixel(0.1));
        pen.setColor(QColor::fromRgb(QRandomGenerator::global()->bounded(0, 255),
                                       QRandomGenerator::global()->bounded(0, 255),
                                       QRandomGenerator::global()->bounded(0, 255)));
        st->setGeomType(type);
        st->setPen(pen);
        return st;
    }
    case LIPGeometryType::LIPPolygon:
    {
        QBrush brush;
        pen.setWidthF(0);
        brush.setColor(QColor::fromRgb(QRandomGenerator::global()->bounded(0, 255),
                                       QRandomGenerator::global()->bounded(0, 255),
                                       QRandomGenerator::global()->bounded(0, 255)));
        brush.setStyle(Qt::SolidPattern);
        st->setBrush(brush);
        st->setPen(pen);
        st->setPointSize(1);
        st->setGeomType(type);
        return st;
    }
    default:
        break;
    }

    delete st;
    st=nullptr;
    return nullptr;
}

LIPVectorStyle LIPVectorStyle::getSelectedStyle(LIPVectorStyle *currentStyle)
{
    if (currentStyle==nullptr)
        return LIPVectorStyle();
    QPen pen;
    LIPVectorStyle selectStyle;
    switch (currentStyle->GetGeomType())
    {
    case LIPGeometryType::LIPPoint:
    {
        pen.setWidthF(currentStyle->getPen().widthF()+1);
        pen.setColor(Qt::red);
        QBrush brush;
        brush.setColor(Qt::red);
        brush.setStyle(Qt::SolidPattern);
        selectStyle.setBrush(brush);
        selectStyle.setPen(pen);
        selectStyle.setPointSize(currentStyle->getPointSize()+1);
        selectStyle.setGeomType(LIPGeometryType::LIPPoint);
        selectStyle.setPointType(currentStyle->getPointType());
        return selectStyle;
    }
    case LIPGeometryType::LIPLineString:
    {
        pen.setWidthF(currentStyle->getPen().widthF()+1);
        pen.setColor(Qt::red);
        selectStyle.setGeomType(LIPGeometryType::LIPLineString);
        selectStyle.setPen(pen);
        return selectStyle;
    }
    case LIPGeometryType::LIPPolygon:
    {
        pen.setWidthF(currentStyle->getPen().widthF()+1);
        pen.setColor(Qt::red);
        selectStyle.setBrush(currentStyle->getBrush());
        selectStyle.setPen(pen);
        //selectStyle.setPointSize(1);
        selectStyle.setGeomType(LIPGeometryType::LIPPolygon);
        return selectStyle;
    }
    default:
        break;
    }

    return selectStyle;
}

//LIPVectorStyle *LIPVectorStyle::selectedStyle(LIPVectorStyle* style)
//{
//    LIPVectorStyle* st = style;
//    st->setBrushColor(Qt::red);
//    st->setPenColor(Qt::red);
//    return st;

//}
