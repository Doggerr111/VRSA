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
#include "lippoint.h"
#include "lipattribute.h"

class LIPVectorReader : public QObject
{
    Q_OBJECT
public:
    LIPVectorReader(const LIPTypes::LIPDrivers Driver, const char* new_filename);
    bool Read();
    /**
     *
     * \brief Данная функция считывает координаты векторного слоя
     */
    bool ReadGeometry();
    /**
     *
     * \brief Данная функция возвращает охват слоя
     */
    QRectF ReadBoundingBox();

    static QPair<OGRLayer*, GDALDataset*> readOGRLayer(QString filename);
    static QVector<OGRLayer*> readLayersFromDataset(GDALDataset *ds);
    static LIPGeometryType readGeometryType(OGRLayer* layer);
    static std::map<int, QVector<LIPAttribute>> readAttributes(OGRLayer* l);
    //bool ReadGeometry():

private:
    const LIPTypes::LIPDrivers driver;
    const char* fileName;
    LIPGeometryType geomertyType;

};

#endif // LIPVECTORREADER_H
