#include "liprasterlayer.h"
#include <QDebug>
#include "mainwindow.h"
LIPRasterLayer::LIPRasterLayer(QString fName):
    mFileName{fName},
    mDs{nullptr},
    mPixmapItem{nullptr}
{
    GDALAllRegister();
    int startIndex = mFileName.lastIndexOf('/') + 1; // Находим индекс символа '/' и добавляем 1, чтобы пропустить его
    int endIndex = mFileName.indexOf(".shp");
    mGISName = mFileName.mid(startIndex, endIndex - startIndex);
    QByteArray bytea=mFileName.toLocal8Bit();
    const char *charname=bytea.data();
    mDs = static_cast<GDALDataset*>(GDALOpen(charname, GA_ReadOnly));


    if (mDs == nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Ошибка создания/открытия растрового слоя", 2000, messageStatus::Error);
        delete this;
        return;
    }

    double geoTransform[6];
    if (mDs->GetGeoTransform(geoTransform) != CE_None) {
        LIPWidgetManager::getInstance().showMessage("Растровые слои без геопривязки не поддерживаются", 2000, messageStatus::Error);
        delete this;
        return;
    }

    // Получение ширины и высоты изображения
    mWidth = mDs->GetRasterXSize();
    mHeight = mDs->GetRasterYSize();
    mNumPixels = mWidth*mHeight;
    mImage= QImage(mWidth, mHeight, QImage::Format_ARGB32_Premultiplied);
    for (int i=0; i<mDs->GetRasterCount(); i++)
    {
        auto band = mDs->GetRasterBand(i+1);
        mBands.append(band);
        if (i>2) //считаем только для первых трех слоёв
            continue;
        ushort* buffer = new ushort[mNumPixels];
        auto error = band->RasterIO(GF_Read, 0, 0, mWidth, mHeight, buffer, mWidth, mHeight, GDT_UInt16, 0, 0);
        if (error != CE_None)
        {
            LIPWidgetManager::getInstance().showMessage("Ошибка чтения канала №" + QString::number(i+1), 1000, Neutral);
        }

        mRasterData.push_back(buffer);
    }

    //формируем изображение

    LIPRasterRGBStyle *rgbStyle = new LIPRasterRGBStyle;
    mStyle=rgbStyle;
    if (mBands.count()==1)
    {
        rgbStyle->bandIndex1=rgbStyle->bandIndex2=rgbStyle->bandIndex3=1;
        double* minMaxValue1 = new double[1];
        mBands.at(0)->ComputeRasterMinMax(false, minMaxValue1);
        rgbStyle->minPixelValue_band1=rgbStyle->minPixelValue_band2=rgbStyle->minPixelValue_band3=minMaxValue1[0];
        rgbStyle->maxPixelValue_band1=rgbStyle->maxPixelValue_band2=rgbStyle->maxPixelValue_band3=minMaxValue1[1];

    }
    if (mBands.count()==2)
    {
        rgbStyle->bandIndex1=1;
        rgbStyle->bandIndex2=rgbStyle->bandIndex3=2;
        double* minMaxValue1 = new double[1];
        mBands.at(0)->ComputeRasterMinMax(false, minMaxValue1);
        double* minMaxValue2 = new double[1];
        mBands.at(1)->ComputeRasterMinMax(false, minMaxValue2);
        rgbStyle->minPixelValue_band1=minMaxValue1[0];
        rgbStyle->minPixelValue_band2=rgbStyle->minPixelValue_band3=minMaxValue2[0];
        rgbStyle->maxPixelValue_band1=minMaxValue1[1];
        rgbStyle->maxPixelValue_band2=rgbStyle->maxPixelValue_band3=minMaxValue2[1];
    }
    if (mBands.count()>=3)
    {
        rgbStyle->bandIndex1=1;
        rgbStyle->bandIndex2=2;
        rgbStyle->bandIndex3=3;
        double* minMaxValue1 = new double[1];
        mBands.at(0)->ComputeRasterMinMax(false, minMaxValue1);
        double* minMaxValue2 = new double[1];
        mBands.at(1)->ComputeRasterMinMax(false, minMaxValue2);
        double* minMaxValue3 = new double[1];
        mBands.at(2)->ComputeRasterMinMax(false, minMaxValue3);
        rgbStyle->minPixelValue_band1=minMaxValue1[0];
        rgbStyle->minPixelValue_band2=minMaxValue2[0];
        rgbStyle->minPixelValue_band3=minMaxValue3[0];
        rgbStyle->maxPixelValue_band1=minMaxValue1[1];
        rgbStyle->maxPixelValue_band2=minMaxValue2[1];
        rgbStyle->maxPixelValue_band3=minMaxValue3[1];
    }
    setRasterStyle(rgbStyle);




    //QPixmap pixmap = QPixmap::fromImage(image);

    ///Создание QGraphicsPixmapItem с преобразованием геопривязки
    // Получение преобразования геопривязки

    mGeoTransform.translate(geoTransform[0], geoTransform[3]);
    mGeoTransform.scale(geoTransform[1], geoTransform[5]);
    //pixmapItem->setOffset(geoTransform[0], geoTransform[3]);
    QTransform transform;
    transform.translate(geoTransform[0], geoTransform[3]);
    transform.scale(geoTransform[1], geoTransform[5]);
    mPixmapItem = new QGraphicsPixmapItem();
    mPixmapItem->setTransform(transform);
    composeRGBImage();
    LIPWidgetManager::getInstance().getMainWindow()->addRasterLayer(this);
    //pixmapItem->setTransform(transform);
}

LIPRasterLayer::~LIPRasterLayer()
{
    GDALClose(mDs);

    mDs=nullptr;
    delete mPixmapItem;
    mPixmapItem=nullptr;
    for(auto data: mRasterData)
    {
        delete[] data;
    }
    mRasterData.clear();

}

QImage LIPRasterLayer::mergeBands()
{
    QVector<unsigned char*> data;
    int width = mDs->GetRasterXSize();
    int height = mDs->GetRasterYSize();
    int numPixels = width * height;
    for (auto band: mBands)
    {
         unsigned char* bandData = new unsigned char[numPixels];
         band->RasterIO(GF_Read, 0, 0, width, height, bandData, width, height, GDT_Byte, 0, 0);

    }
    QImage mergedImage(width, height, QImage::Format_RGB888);

    for (int i = 0; i < numPixels; ++i) {
        QRgb color = qRgb(data[0][i], data[1][i], data[2][i]);
        mergedImage.setPixel(i % width, i / width, color);
    }
    return mergedImage;
}

bool LIPRasterLayer::readGeoTransform()
{
    // Получение преобразования геопривязки
    double geoTransform[6];
    if (mDs->GetGeoTransform(geoTransform) == CE_None) {
        LIPWidgetManager::getInstance().showMessage("Растровые слои без геопривязки не поддерживаются", 2000, messageStatus::Error);
        return false;
    }
    mGeoTransform.translate(geoTransform[0], geoTransform[3]);
    mGeoTransform.scale(geoTransform[1], geoTransform[5]);
}

bool LIPRasterLayer::composeRGBImage()
{
    if (mBands.isEmpty())
        return false;
//    if (mBands.count()==1)
//    {
//        double* minMaxValue = new double[1];
//        mBands.at(0)->ComputeRasterMinMax(false, minMaxValue);
//        for (int i = 0; i < mNumPixels; i++)
//        {
//            auto pixelValue1 = mRasterData[0][i];
//            int normalizedBrightness = (pixelValue1 - minMaxValue[0]) * 255 / (minMaxValue[1] - minMaxValue[0]);
//            if (normalizedBrightness>255)
//                normalizedBrightness=255;
//            QRgb color = qRgb(normalizedBrightness, normalizedBrightness, normalizedBrightness);
//            mImage.setPixel(i % mWidth, i / mWidth, color);
//        }
//        return true;
//    }
//    else if (mBands.count()==2)
//    {
//        double* minMaxValue1 = new double[1];
//        mBands.at(0)->ComputeRasterMinMax(false, minMaxValue1);
//        double* minMaxValue2 = new double[1];
//        mBands.at(1)->ComputeRasterMinMax(false, minMaxValue2);
//        for (int i = 0; i < mNumPixels; i++)
//        {
//            auto pixelValue1 = mRasterData[0][i];
//            auto pixelValue2 = mRasterData[1][i];
//            int normalizedBrightness1 = (pixelValue1 - minMaxValue1[0]) * 255 / (minMaxValue1[1] - minMaxValue1[0]);
//            int normalizedBrightness2 = (pixelValue2 - minMaxValue2[0]) * 255 / (minMaxValue2[1] - minMaxValue2[0]);
//            if (normalizedBrightness1>255)
//                normalizedBrightness1=255;
//            if (normalizedBrightness2>255)
//                normalizedBrightness2=255;
//            QRgb color = qRgb(normalizedBrightness1, normalizedBrightness2, normalizedBrightness2);
//            mImage.setPixel(i % mWidth, i / mWidth, color);

//            }

//    }
//    else if (mBands.count()>=3)
//    {

    for (int i = 0; i < mNumPixels; i++)
    {
        auto pixelValue1 = mRasterData[mRGBStyle->bandIndex1-1][i];
        auto pixelValue2 = mRasterData[mRGBStyle->bandIndex2-1][i];
        auto pixelValue3 = mRasterData[mRGBStyle->bandIndex3-1][i];

        int normalizedBrightness1=0;
        int normalizedBrightness2=0;
        int normalizedBrightness3=0;
        if (mRGBStyle->maxPixelValue_band1 - mRGBStyle->minPixelValue_band1!=0)
            normalizedBrightness1 = (pixelValue1 - mRGBStyle->minPixelValue_band1) * 255 / (mRGBStyle->maxPixelValue_band1 - mRGBStyle->minPixelValue_band1);
        if (mRGBStyle->maxPixelValue_band2 - mRGBStyle->minPixelValue_band2!=0)
            normalizedBrightness2 = (pixelValue2 - mRGBStyle->minPixelValue_band2) * 255 / (mRGBStyle->maxPixelValue_band2 - mRGBStyle->minPixelValue_band2);
        if (mRGBStyle->maxPixelValue_band3 - mRGBStyle->minPixelValue_band3!=0)
            normalizedBrightness3 = (pixelValue3 - mRGBStyle->minPixelValue_band3) * 255 / (mRGBStyle->maxPixelValue_band3 - mRGBStyle->minPixelValue_band3);


        if (normalizedBrightness1>255)
            normalizedBrightness1=255;
        if (normalizedBrightness2>255)
            normalizedBrightness2=255;
        if (normalizedBrightness3>255)
            normalizedBrightness3=255;
        QRgb color = qRgb(normalizedBrightness1, normalizedBrightness2, normalizedBrightness3);
        mImage.setPixel(i % mWidth, i / mWidth, color);

    }
    mPixmapItem->setPixmap(QPixmap::fromImage(mImage));




}

int LIPRasterLayer::getBandCount()
{
    return mBands.count();
}

QPixmap LIPRasterLayer::getPixmap()
{
    return QPixmap::fromImage(mImage);
}

QGraphicsPixmapItem *LIPRasterLayer::getPixmapItem()
{
    return mPixmapItem;
}

QString LIPRasterLayer::getGISName()
{
    return mGISName;
}

QString LIPRasterLayer::getFileName()
{
    return mFileName;
}

GDALDataset *LIPRasterLayer::getDataSet()
{
    return mDs;
}

QRectF LIPRasterLayer::getBoundingBox()
{
    double geoTransform[6];
    mDs->GetGeoTransform(geoTransform);
    double minX = geoTransform[0];
    double maxY = geoTransform[3];
    double maxX = geoTransform[0] + geoTransform[1] * mDs->GetRasterXSize();
    double minY = geoTransform[3] + geoTransform[5] * mDs->GetRasterYSize();
    return QRectF(QPointF(minX, minY), QSizeF(maxX - minX, maxY - minY));

}

bool LIPRasterLayer::setRasterStyle(LIPRasterStyle *style)
{
    mStyle=style;
    auto rgbStyle = dynamic_cast<LIPRasterRGBStyle*>(style);
    if (rgbStyle!=nullptr)
    {
        mRGBStyle=rgbStyle;
        //composeRGBImage();
    }
}

LIPRasterStyle *LIPRasterLayer::getStyle()
{
    return mStyle;
}

void LIPRasterLayer::setVisible(bool fl)
{
     if (fl)
         mPixmapItem->show();
     else
     {
         mPixmapItem->hide();
     }
}

void LIPRasterLayer::setZValue(int value)
{
    if (mPixmapItem)
        mPixmapItem->setZValue(value);
}

void LIPRasterLayer::update()
{
    if (!mStyle)
        return;
    LIPRasterRGBStyle *rgbStyle = dynamic_cast<LIPRasterRGBStyle*>(mStyle);
    if (rgbStyle)
    {
        for (int i = 0; i < mNumPixels; i++)
        {
            auto pixelValue1 = mRasterData[mRGBStyle->bandIndex1-1][i];
            auto pixelValue2 = mRasterData[mRGBStyle->bandIndex2-1][i];
            auto pixelValue3 = mRasterData[mRGBStyle->bandIndex3-1][i];
            int normalizedBrightness1 = (pixelValue1 - mRGBStyle->minPixelValue_band1) * 255 / (mRGBStyle->maxPixelValue_band1 - mRGBStyle->minPixelValue_band1);
            int normalizedBrightness2 = (pixelValue2 - mRGBStyle->minPixelValue_band2) * 255 / (mRGBStyle->maxPixelValue_band2 - mRGBStyle->minPixelValue_band2);
            int normalizedBrightness3 = (pixelValue3 - mRGBStyle->minPixelValue_band3) * 255 / (mRGBStyle->maxPixelValue_band3 - mRGBStyle->minPixelValue_band3);
            if (normalizedBrightness1>255)
                normalizedBrightness1=255;
            if (normalizedBrightness2>255)
                normalizedBrightness2=255;
            if (normalizedBrightness3>255)
                normalizedBrightness3=255;
            QRgb color = qRgb(normalizedBrightness1, normalizedBrightness2, normalizedBrightness3);
            mImage.setPixel(i % mWidth, i / mWidth, color);

        }
        mPixmapItem->setPixmap(QPixmap::fromImage(mImage));

    }
}
