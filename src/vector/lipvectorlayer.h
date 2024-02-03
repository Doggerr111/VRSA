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
    static QVector<LIPAttribute> stringValToAttrs(QVector<QString> names, QVector<QString> values, QVector<LIPAttributeType> types);
    LIPVectorStyle *getStyle();
    OGRLayer *getOGRLayer();
    std::map<int, QVector<LIPAttribute>> getAttributes();
    bool setCoordinateSystem(LIPCoordinateSystem *targetCRS);

public:
    virtual QString returnGISName();
    virtual ~LIPVectorLayer();
    virtual void addFeature(QVector<QPointF> coords, QVector<LIPAttribute> attrs);
    virtual void setVisible(bool=true);
    virtual bool reproject(LIPCoordinateSystem *targetCRS);
    virtual bool reproject(LIPCoordinateSystem *sourceCRS, LIPCoordinateSystem *targetCRS);
    virtual void update();
    virtual void setMapFeatures();
    virtual void selectFeature(int index);

signals:
    void needRepaint(LIPVectorLayer*);

public slots:
    virtual void setSceneScaleFactor(double factor);

protected:
    OGRLayer *layer;
    GDALDataset *dS;
    QString fileName;
    QVector<LIPAttributeType> attributeTypes;
    QVector<QString> attributeNames;
    double mScaleFactor;
    LIPVectorStyle *mStyle;
    LIPCoordinateSystem* mCRS;





};

#endif // LIPVECTORLAYER_H
