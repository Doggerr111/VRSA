#ifndef TESTLIPVECTORLAYER_H
#define TESTLIPVECTORLAYER_H
//#include "vector/lipvectorlayer.h"
//#include "vector/lippointgraphicsitem.h"
//class moc_LIPPointLayer: public LIPVectorLayer
//{
//public:
//    moc_LIPPointLayer(OGRLayer *l, QString name, QString fileName, GDALDataset* dataset);
//    ~moc_LIPPointLayer();
//    QVector<LIPPoint*> returnCords();

//    QVector<LIPPointGraphicsItem*> returnMapFeatures();
//    void addFeature(LIPPoint* p);
//    void setFileName(QString path);
//    QString getFileName();
//    void setStyle(LIPVectorStyle *style);

//    void setMapFeatures() override;
//    void addFeature(QVector<QPointF> coords, QVector<LIPAttribute> attrs) override;
//    QString returnGISName() override;
//    bool reproject(LIPCoordinateSystem *targetCRS) override;
//    bool reproject(LIPCoordinateSystem *sourceCRS, LIPCoordinateSystem *targetCRS) override;
//    void update() override;


//public slots:
//    void setSceneScaleFactor(double factor) override;



//private:
//    //void createMapFeatures();
//    //OGRLayer *layer;
//    QString GISName;
//    QVector<LIPPoint*> coordinates;
//    QString fileName;
//    QVector<LIPPointGraphicsItem*> mapFeatures;







//    // LIPVectorLayer interface
//public:
//    void setVisible(bool=true) override;

//    // LIPVectorLayer interface
//public:
//    void setZValue(int zValue);
//};

//class TestLIPVectorLayer
//{
//public:
//    TestLIPVectorLayer();
//};

#endif // TESTLIPVECTORLAYER_H
