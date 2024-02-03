#include "lippointrenderer.h"

LIPPointRenderer::LIPPointRenderer(LIPPoint* point):
    LIPFeatureRenderer(LIPGeometryType::LIPPoint)
  , mPoint{point}
{

}

void LIPPointRenderer::renderFeature()
{
    //TODO calculate mImgSize before
    //TODO create a class with Style
    mFeatureImage=QImage(mImgSize,QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&mFeatureImage);
    painter.setPen(mPen);
    painter.setBrush(mBrush);

}
