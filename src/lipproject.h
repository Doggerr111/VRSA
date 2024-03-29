#ifndef LIPPROJECT_H
#define LIPPROJECT_H
#include <QObject>
#include "vector/lipvectorlayer.h"
#include "lipcoordinatesystem.h"
#include "liprasterlayer.h"
#include <QFile>
#include <QAction>
class LIPProject : public QObject
{
    Q_OBJECT
public:
    static LIPProject &getInstance();

    struct guiSettings
    {
        QVector<QAction*> mainToolBarActions;
    };

    //vector layers
    void addVectorLayer(LIPVectorLayer* vect);
    void addVectorLayers(QVector<LIPVectorLayer*> vects);
    void setVectorLayers(QVector<LIPVectorLayer*> vects);
    bool setActiveLayer(LIPVectorLayer* vect);
    void setActiveLayer(int index);
    void setActiveLayer(QString name);
    void deleteVectorByPath(QString path);

    QVector<LIPVectorLayer*> getVectorLayers();
    LIPVectorLayer* getVecorLayerByIndx(int index);
    LIPVectorLayer* getVectorLayerByName(QString name);
    LIPVectorLayer* getVectorLayerByPath(QString path);
    LIPVectorLayer* getActiveLayer();

    //rasterLayers
    void addRasterLayer(LIPRasterLayer* rasterLayer);
    void addRasterLayers(QVector<LIPVectorLayer*> rasterLayers);
    void setRasterLayers(QVector<LIPVectorLayer*> rasterLayers);
    void deleteRasterLayerByPath(QString path);

    QVector<LIPRasterLayer*> getRasterLayers();
    LIPRasterLayer* getRasterLayerByIndx(int index);
    LIPRasterLayer* getRasterLayerByName(QString name);
    LIPRasterLayer *getRasterLayerByPath(QString path);

    //database
    void setActivePostGISConnection(GDALDataset* ds);
    void disconnectPostGISConnection();
    /** Получение набора слоев из базы данных. Функция возвращает nullptr, если нет активного соединения*/
    GDALDataset *getPostGISDataSet();


    bool isDatasetInUse(GDALDataset* dS);

    //SRC
    bool addCoordinateSystem(LIPCoordinateSystem *crs);
    QVector<LIPCoordinateSystem*> getCoordinateSystems();
    bool setProjectCRS(LIPCoordinateSystem* s);
    LIPCoordinateSystem *getProjectCRS();

    //project save
    bool saveProject(QString filename);
    bool openProject(QString filename);


    //gui
    void addAction(QAction* action);
    void deleteAction(QAction* action);
    bool isActionAdded(QAction* action);
    void setActions(QVector<QAction*>);

    //path settings
    void setVectorDataFolder(QString);
    void setRasterDataFolder(QString);

    QString getVectorDataFolder();
    QString getRasterDataFolder();


    //map settings
    void setMapCanvasColor(QColor col);
    void setAntiAliasing(bool flag);

    QColor getMapCanvasColor();
    bool isAntiAliasingOn();


    //from mainwindow
    void setSelectFeatureFlag(bool fl);
    bool isSelectingFeatures();

public slots:
    void redrawNeeded(double);


signals:
    void newCRS();
    void crsChanged();
private:
    LIPVectorLayer* activeLayer; //
    QVector<LIPVectorLayer*> vectorLayers;
    QVector<LIPRasterLayer*> rasterLayers;
    QVector<LIPCoordinateSystem*> CRSystems;
    GDALDataset* mActivePostGISConnection;
    LIPCoordinateSystem* mProjectCRS;
    guiSettings mGUISettings;
    QString mVectorFolder;
    QString mRasterFolder;
    QColor mMapCanvasColor = Qt::white;
    bool mAntiAliasingFlag = false;
    bool mIsSelectingFeatures = false;


private:
    LIPProject() = default;
    ~LIPProject() = default;
    LIPProject(const LIPProject&) = delete;
    LIPProject& operator=(const LIPProject&) = delete;

};

#endif // LIPPROJECT_H
