#ifndef LIPPROJECT_H
#define LIPPROJECT_H
#include <QObject>
#include <vector/lipvectorlayer.h>
#include <lipcoordinatesystem.h>
class LIPProject : public QObject
{
    Q_OBJECT
public:
    static LIPProject &getInstance();

    //layers
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

    //SRC
    bool addCoordinateSystem(LIPCoordinateSystem *crs);

    QVector<LIPCoordinateSystem*> getCoordinateSystems();

public slots:
    void redrawNeeded(double);

signals:
    void newCRS();
private:
    LIPVectorLayer* activeLayer; //
    QVector<LIPVectorLayer*> vectorLayers;

    QVector<LIPCoordinateSystem*> CRSystems;

private:
    LIPProject() = default;
    ~LIPProject() = default;
    LIPProject(const LIPProject&) = delete;
    LIPProject& operator=(const LIPProject&) = delete;

};

#endif // LIPPROJECT_H
