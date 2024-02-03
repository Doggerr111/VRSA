#ifndef LIPLAYERCREATOR_H
#define LIPLAYERCREATOR_H
#include "gdal.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include "liptypes.h"
#include "QObject"
#include <QVariant>
#include "vector/lippointlayer.h"
#include "vector/liplinelayer.h"
#include "vector/lippolygonlayer.h"
#include "lipcoordinatesystem.h"


class LIPLayerCreator
{
public:
    LIPLayerCreator(LIPGeometryType type, QString filename, QString nameGIS, LIPCoordinateSystem* system);
    void createAttribute(LIPAttributeType type, QString name);
    void setAttribute(LIPAttributeType type, QVariant attribute, QString name);
    void setName(QString f);
    void setCodec();
    void setGeometry();
    OGRLayer* returnOGRLayer();
    LIPVectorLayer* returnLayer();
private:
    char fileName;
    QString fileNameAsString;
    OGRLayer *layer;
    LIPPointLayer* pointLayer;
    LIPLineLayer* lineLayer;
    LIPPolygonLayer* polyLayer;
    LIPGeometryType geomType;
    QString mainName;
    GDALDataset *ds;

};

#endif // LIPLAYERCREATOR_H
