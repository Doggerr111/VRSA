#ifndef LIPLINELAYER_H
#define LIPLINELAYER_H
#include "gdal.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include <QString>
#include "lippoint.h"
#include <vector/lipvectorlayer.h>
#include <vector/liplinegraphicsitem.h>

class LIPLineLayer: public LIPVectorLayer
{
public:
    /**
     * \brief Данный конструктор применяется, если мы создаем новый слой
     */
    LIPLineLayer(OGRLayer *l, QString name, QString fileName, GDALDataset* ds);
    ~LIPLineLayer();
    /**
     * \brief Данный конструктор применяется, если мы загружаем слой
     */
    LIPLineLayer(QString fileName);
    QString returnGISName() override;
    void setFileName(QString path);
    QString getFileName();
    void setMapFeatures() override;
    QVector<LIPLineGraphicsItem*> returnMapFeatures();

    QVector<QVector<LIPPoint*>>  returnCords();



    void setStyle(LIPVectorStyle *style);
public slots:
    void setSceneScaleFactor(double factor) override;
private:
    //OGRLayer *layer = nullptr;
    QString GISName;
    QVector<QVector<LIPPoint*>> coordinates;
    QRectF boundingRect;
    QString fileName;

    QVector<LIPLineGraphicsItem*> mapFeatures;





    // LIPVectorLayer interface
public:
    void addFeature(QVector<QPointF> coords, QVector<LIPAttribute> attrs);

    // LIPVectorLayer interface
public:
    void setVisible(bool=true) override;
};

#endif // LIPLINELAYER_H
