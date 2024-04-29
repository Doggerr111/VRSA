#ifndef LIPCUTRASTERLAYER_H
#define LIPCUTRASTERLAYER_H
#include "liprasterlayer.h"
#include "vector/lippolygonlayer.h"
#include <gdal/cpl_conv.h>
#include <gdal/gdal_priv.h>
#include <gdalwarper.h>
#include <gdal_utils.h>
class LIPCutRasterLayer
{
public:
    LIPCutRasterLayer();
    static bool cutRasterByVector(LIPRasterLayer* rLayer, LIPPolygonLayer* vLayer, QString outPath);
};

#endif // LIPCUTRASTERLAYER_H
