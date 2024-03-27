#ifndef LIPVECTORREADER_H
#define LIPVECTORREADER_H

#include <QObject>
#include "gdal.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include "QDebug"
#include "liptypes.h"
#include "QMessageBox"
//#include "lipvectorlayer.h"
#include "vector/lippoint.h"
#include "vector/lipattribute.h"
#include "lipwidgetmanager.h"

class LIPVectorReader : public QObject
{
    Q_OBJECT
public:
    LIPVectorReader(const LIPTypes::LIPDrivers Driver, const char* new_filename);
    bool Read();
    /**
     *
     * Данная функция считывает координаты векторного слоя
     */
    bool ReadGeometry();
    /**
     *
     * Данная функция возвращает охват слоя
     */
    QRectF ReadBoundingBox();

    static QPair<OGRLayer*, GDALDataset*> readOGRLayer(QString filename);
    static QVector<OGRLayer*> readLayersFromDataset(GDALDataset *ds);
    static LIPGeometryType readGeometryType(OGRLayer* layer);
    static std::map<int, QVector<LIPAttribute>> readAttributes(OGRLayer* l);


    /** Функция для чтения нескольких векторных слоев из набора путей QStringList. Функция сразу же создает обьекты LIPVectorLayer
    и, соответственно, добавляет их на карту*/
    static bool readLayersFromStringList(QStringList paths);
    //bool ReadGeometry():

private:
    const LIPTypes::LIPDrivers driver;
    const char* fileName;
    LIPGeometryType geomertyType;

};

#endif // LIPVECTORREADER_H
