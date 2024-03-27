#ifndef LIPVECTORLAYER_H
#define LIPVECTORLAYER_H
#include <qstring.h>
#include <QObject>
#include "gdal.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include "liptypes.h"
//#include "lipattribute.h"
#include "lipattribute.h"
#include "lipvectorstyle.h"
#include "lipvectorreader.h"
#include "lipcoordinatesystem.h"
#include "lipwidgetmanager.h"
#include "ogr_spatialref.h"

class LIPPointLayer;
class LIPLineLayer;
class LIPPolygonLayer;

class LIPVectorLayer: public QObject
{
    Q_OBJECT
public:
    LIPVectorLayer(OGRLayer *l, QString path, GDALDataset *dataset);
    QString returnFileName();
    QVector<LIPAttributeType> getAttributeTypes();
    QVector<QString> getAttributeNames();
    QStringList getAttributesNamesAsList();
    QRectF getBoundingBox();
    LIPCoordinateSystem *getCRS();
    void setCRS(LIPCoordinateSystem* crs);
    static QVector<LIPAttribute> stringValToAttrs(QVector<QString> names, QVector<QString> values, QVector<LIPAttributeType> types);
    LIPVectorStyle *getStyle();
    OGRLayer *getOGRLayer();
    GDALDataset *getDataSet();
    std::map<int, QVector<LIPAttribute>> getAttributes();
    bool setCoordinateSystem(LIPCoordinateSystem *targetCRS);


    LIPPointLayer* toPointLayer();
    LIPLineLayer* toLineLayer();
    LIPPolygonLayer* toPolygonLayer();


public:
    virtual QString returnGISName();
    virtual ~LIPVectorLayer();
    virtual void addFeature(QVector<QPointF> coords, QVector<LIPAttribute> attrs);
    virtual void setVisible(bool=true);
    virtual bool reproject(LIPCoordinateSystem *targetCRS);
    virtual bool reproject(LIPCoordinateSystem *sourceCRS, LIPCoordinateSystem *targetCRS);
    virtual bool reproject(LIPCoordinateSystem *targetCRS, QString fileName);
    virtual void update();
    virtual void setMapFeatures();
    virtual void selectFeature(int index);
    virtual void setZValue(int zValue);


signals:
    void needRepaint();

public slots:
    virtual void setSceneScaleFactor(double factor);
    virtual void flyReprojection();

protected:
    OGRLayer *layer;
    GDALDataset *dS;
    QString fileName;
    QVector<LIPAttributeType> attributeTypes;
    QVector<QString> attributeNames;
    double mScaleFactor;
    LIPVectorStyle *mStyle;
    LIPCoordinateSystem* mCRS;
    int mZValue;





};

#endif // LIPVECTORLAYER_H
