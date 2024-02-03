#ifndef LIPPOINTLAYER_H
#define LIPPOINTLAYER_H
#include "gdal.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include <QString>
#include "vector/lipvectorlayer.h"
#include "lippoint.h"
#include "vector/lippointgraphicsitem.h"
#include <QObject>
#include "lipwidgetmanager.h"

class LIPPointLayer: public LIPVectorLayer
{

public:
    LIPPointLayer(OGRLayer *l, QString name, QString fileName, GDALDataset* dataset);
    ~LIPPointLayer();
    QVector<LIPPoint*> returnCords();

    QVector<LIPPointGraphicsItem*> returnMapFeatures();
    void addFeature(LIPPoint* p);
    void setFileName(QString path);
    QString getFileName();
    void setStyle(LIPVectorStyle *style);

    void setMapFeatures() override;
    void addFeature(QVector<QPointF> coords, QVector<LIPAttribute> attrs) override;
    QString returnGISName() override;
    bool reproject(LIPCoordinateSystem *targetCRS) override;
    bool reproject(LIPCoordinateSystem *sourceCRS, LIPCoordinateSystem *targetCRS) override;
    void update() override;


public slots:
    void setSceneScaleFactor(double factor) override;



private:
    //void createMapFeatures();
    //OGRLayer *layer;
    QString GISName;
    QVector<LIPPoint*> coordinates;
    QString fileName;
    QVector<LIPPointGraphicsItem*> mapFeatures;







    // LIPVectorLayer interface
public:
    void setVisible(bool=true) override;
};

#endif // LIPPOINTLAYER_H
