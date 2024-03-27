#ifndef LIPRASTERPROCESSING_H
#define LIPRASTERPROCESSING_H

#include "liprasterlayer.h"
#include "vector/lippolygonlayer.h"
#include <gdal/cpl_conv.h>
#include <gdal/gdal_priv.h>
#include <gdalwarper.h>
class LIPRasterProcessing
{
public:
    LIPRasterProcessing();
    static void createContours(LIPRasterLayer *rasterLayer, int bandIndex, double interval, double baseValue, QString outputFileName);
};

#endif // LIPRASTERPROCESSING_H
