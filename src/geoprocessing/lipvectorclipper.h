#ifndef LIPVECTORCLIPPER_H
#define LIPVECTORCLIPPER_H
#include "gdal.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"

#include <QString>
#include <vector/lipvectorlayer.h>
#include <vector/lippointgraphicsitem.h>
#include <QObject>
#include "lipwidgetmanager.h"
#include <lippolygonlayer.h>
#include <liplinelayer.h>
#include "liplayercreator.h"

/** Обрезка вектора по обьектам другого вектора (полигона) */
class LIPVectorClipper
{
public:
    //

    LIPVectorClipper(LIPVectorLayer* clippedLayer, LIPVectorLayer* polyLayer);
    /** обрезает layer, используя объекты полигонального слоя polyLayer */
    static OGRLayer* clip(LIPVectorLayer *layer, LIPVectorLayer *polyLayer);
};

#endif // LIPVECTORCLIPPER_H
