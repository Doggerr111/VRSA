#ifndef LIPFEATURERENDERER_H
#define LIPFEATURERENDERER_H
#include <liptypes.h>
#include <QSize>
#include <QPainter>
#include <QPen>
#include <QImage>

class LIPFeatureRenderer
{
public:
    LIPFeatureRenderer(LIPGeometryType type);
    //convert mm to pixels
    bool MMtoPixel(double mm);
    //convert pixels to mm
    bool PixeltoMM(double px);

    void setPen(QPen pen);

    void setBrush(QBrush brush);

    virtual void renderFeature();

    QImage getFeatureImage();


protected:
    LIPGeometryType mGeomType;
    //img_size
    QSize mImgSize;
    //Pen
    QPen mPen;
    QBrush mBrush;
    //result image
    QImage mFeatureImage;


};

#endif // LIPFEATURERENDERER_H
