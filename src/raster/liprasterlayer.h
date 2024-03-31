#ifndef LIPRASTERLAYER_H
#define LIPRASTERLAYER_H
#include "lipwidgetmanager.h"
#include <gdal.h>
#include <gdal_priv.h>
#include <QGraphicsPixmapItem>
#include "liprasterstyle.h"

//struct LIPRasterRGBStyle
//{
//    //индексы каналов, используемые для отображения
//    int bandIndex1;
//    int bandIndex2;
//    int bandIndex3;

//    int minPixelValue_band1;
//    int maxPixelValue_band1;

//    int minPixelValue_band2;
//    int maxPixelValue_band2;

//    int minPixelValue_band3;
//    int maxPixelValue_band3;
//};

class LIPRasterLayer: public QObject
{

    Q_OBJECT
public:
    LIPRasterLayer(QString filename);
    ~LIPRasterLayer();
    QImage mergeBands();
    bool readGeoTransform();
    bool applyRGB();
    bool composeRGBImage();
    int getBandCount();

    QPixmap getPixmap();
    QGraphicsPixmapItem *getPixmapItem();
    QString getGISName();
    QString getFileName();
    GDALDataset *getDataSet();
    QRectF getBoundingBox();
    bool setRasterStyle(LIPRasterStyle *style);
    LIPRasterStyle *getStyle();
    void setVisible(bool fl);
    void setZValue(int value);
public slots:
    void update();
private:
    QString mFileName;
    GDALDataset* mDs;
    QVector<GDALRasterBand*> mBands;
    QTransform mGeoTransform;
    QGraphicsPixmapItem *mPixmapItem;

    QImage mImage;
    QPixmap mPixmap;
    std::vector<ushort*> mRasterData;
    int mWidth;
    int mHeight;
    int mNumPixels;
    LIPRasterRGBStyle *mRGBStyle;
    LIPRasterStyle *mStyle;
    QString mGISName;


};



#endif // LIPRASTERLAYER_H
