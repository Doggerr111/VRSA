#ifndef LIPRASTERSTYLE_H
#define LIPRASTERSTYLE_H


class LIPRasterStyle
{
public:
    LIPRasterStyle();
    virtual ~LIPRasterStyle();

};


class LIPRasterRGBStyle:public LIPRasterStyle
{
public:
    LIPRasterRGBStyle();
    virtual ~LIPRasterRGBStyle();
    //индексы каналов, используемые для отображения
    int bandIndex1;
    int bandIndex2;
    int bandIndex3;

    int minPixelValue_band1;
    int maxPixelValue_band1;

    int minPixelValue_band2;
    int maxPixelValue_band2;

    int minPixelValue_band3;
    int maxPixelValue_band3;
};

#endif // LIPRASTERSTYLE_H
