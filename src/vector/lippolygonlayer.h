#ifndef LIPPOLYGONLAYER_H
#define LIPPOLYGONLAYER_H
#include "gdal.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include <QString>
#include "lipvectorlayer.h"
#include "vector/lippoint.h"
#include "vector/lippolygongraphicsitem.h"
#include "vector/lipattribute.h"
class LIPPolygonLayer: public LIPVectorLayer
{
public:
    LIPPolygonLayer(OGRLayer *l, QString name, QString fileName, GDALDataset* ds);
    ~LIPPolygonLayer();
    QString returnGISName() override;
    QVector<QVector<LIPPoint*>>  returnCords();
    void setFileName(QString path);
    QString getFileName();
    void setMapFeatures() override;
    void setVisible(bool = true) override;
    QVector<LIPPolygonGraphicsItem*> returnMapFeatures();
    void addFeature(QVector<QPointF> coords, QVector<LIPAttribute> attrs) override;
    void selectFeature(int index) override;
    void setStyle(LIPVectorStyle *style);
public slots:
    void setSceneScaleFactor(double factor) override;
    void itemClicked(int ind);

private:
    QVector<QVector<LIPPoint*>> coordinates;
    //OGRLayer *layer = nullptr;
    QString GISName;
    QString fileName;

    QVector<LIPPolygonGraphicsItem*> mapFeatures;


    // LIPVectorLayer interface
public:
    void setZValue(int zValue);

    // LIPVectorLayer interface
public:
    void update();

    // LIPVectorLayer interface
public:
    void deselectItems();
};

#endif // LIPPOLYGONLAYER_H
