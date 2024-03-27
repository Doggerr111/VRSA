#ifndef LIPRASTERTRANSFORM_H
#define LIPRASTERTRANSFORM_H

#include "liprasterlayer.h"
#include "vector/lippolygonlayer.h"
#include <gdal/cpl_conv.h>
#include <gdal/gdal_priv.h>
#include <gdalwarper.h>
class LIPRasterTransform
{
public:
    LIPRasterTransform();
    static bool reproject(LIPRasterLayer *layer, LIPCoordinateSystem *targetCRS, QString fileName);
};

#endif // LIPRASTERTRANSFORM_H
