#include "liplinelayer.h"
#include <QDebug>
#include "mainwindow.h"
LIPLineLayer::LIPLineLayer(OGRLayer *l, QString name, QString fileName, GDALDataset *ds)
    : LIPVectorLayer(l, fileName, ds),
      //layer{l},
      GISName{name},
      fileName{fileName}
{
    if (l==nullptr)
    {

    }
    LIPWidgetManager::getInstance().getMainWindow()->addLayer(this);
}

LIPLineLayer::~LIPLineLayer()
{
    for(int i=0; i<mapFeatures.size(); i++)
    {
        delete mapFeatures.at(i);
    }
    foreach(QVector<LIPPoint*> vec, coordinates)
    {
        foreach(LIPPoint* point, vec)
        {
            delete point;
        }
    }
    mapFeatures.clear();
    coordinates.clear();
}

LIPLineLayer::LIPLineLayer(QString fileName)
    : LIPVectorLayer(nullptr, fileName, nullptr)
{
    QByteArray ba = fileName.toLocal8Bit();
    const char *nameChar = ba.data();
    char** ppszOptions = NULL;
    ppszOptions = CSLSetNameValue(ppszOptions, "ENCODING", "UTF-8");
    CPLSetConfigOption("SHAPE_ENCODING","");
    GDALDataset *shpDS = (GDALDataset *)GDALOpenEx(nameChar, GDAL_OF_VECTOR, NULL, NULL, NULL);
    if (shpDS == NULL)
    {
        qDebug()<<"Error:cant read this shp file: " + QString(fileName);
        delete this;
    }
    int c=shpDS->GetLayers().size();
    qDebug()<<QString::number(c);
    layer = shpDS->GetLayer(0);
}

QString LIPLineLayer::returnGISName()
{
    return GISName;
}


QVector<QVector<LIPPoint*>>  LIPLineLayer::returnCords()
{
    if (layer!=nullptr)
    {
        coordinates.clear();
        layer->GetName();
        OGRFeature *shpFeature;
        layer->ResetReading();
        int counter=0;
        //qDebug()<<shpLayer->GetSpatialRef()->GetEPSGGeogCS();
        while ((shpFeature = layer->GetNextFeature()) != NULL)
        {
            counter++;
//            OGRFeatureDefn *poFDefn = layer->GetLayerDefn();
//            int iField;
//            for (iField = 0; iField < poFDefn->GetFieldCount(); iField++)
//            {
//                OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn(iField);
//                if (poFieldDefn->GetType() == OFTInteger)
//                    qDebug()<<"%d, " +  QString::number(shpFeature->GetFieldAsInteger(iField));
//                else if (poFieldDefn->GetType() == OFTInteger64)
//                    qDebug()<<"%lld, "+QString::number( shpFeature->GetFieldAsInteger64(iField));
//                else if (poFieldDefn->GetType() == OFTReal)
//                    qDebug()<<"%.3f, "+QString::number(shpFeature->GetFieldAsDouble(iField));
//                else if (poFieldDefn->GetType() == OFTString)
//                    qDebug()<<"%s, " + QString(shpFeature->GetFieldAsString(iField));
//                else
//                    qDebug()<<"%s, " + QString(shpFeature->GetFieldAsString(iField));
//            }
            //printf("\n");
            OGRGeometry *poGeometry = shpFeature->GetGeometryRef();
            int count = shpFeature->GetGeomFieldCount();

            for (int i=0;i<shpFeature->GetGeomFieldCount();i++)
            {
                if (poGeometry != NULL)
                {
                    OGRwkbGeometryType type=poGeometry->getGeometryType();

                    QVector<LIPPoint*> vect;
                    OGRLineString *line = (OGRLineString *)poGeometry;
                    for (int i = 0; i < line->getNumPoints(); i++)
                    {
                        LIPPoint *point = new LIPPoint();
                        point->setX(line->getX(i));
                        point->setY(line->getY(i));
                        vect.append(point);
                        //qDebug()<<"wkbLineString %d: x=%g y=%g z=%g\n", i, line->getX(i), line->getY(i), line->getZ(i);
                    }
                    coordinates.append(vect);

                }
            }

        }
        return coordinates;
    }

}

void LIPLineLayer::setStyle(LIPVectorStyle *style)
{
    mStyle=style; //field of vectorLayer
    mStyle->setGeomType(LIPGeometryType::LIPLineString);
    foreach(LIPLineGraphicsItem *item, mapFeatures)
    {
        item->setVectorStyle(style); //задаем стиль для каждого обьекта слоя
    }
}

void LIPLineLayer::setSceneScaleFactor(double factor)
{
    mScaleFactor=factor;
    foreach(LIPLineGraphicsItem* item, mapFeatures)
    {
        item->setScaleFactor(mScaleFactor);
    }
}

void LIPLineLayer::setFileName(QString path)
{
    fileName=path;
}

QString LIPLineLayer::getFileName()
{
    return fileName;
}

void LIPLineLayer::setMapFeatures()
{
    QVector<QVector<LIPPoint*>> vect = returnCords();
    mStyle=LIPVectorStyle::createDefaultVectorStyle(LIPGeometryType::LIPLineString);
    for (int i=0; i<vect.size(); i++)
    {
        LIPLineGraphicsItem *el = new LIPLineGraphicsItem;
        el->setVectorStyle(mStyle);
        el->setPoints(vect.at(i));
        mapFeatures.append(el);

    }

}

QVector<LIPLineGraphicsItem *> LIPLineLayer::returnMapFeatures()
{
    return mapFeatures;
}


void LIPLineLayer::addFeature(QVector<QPointF> coords, QVector<LIPAttribute> attrs)
{
}


void LIPLineLayer::setVisible(bool isVisible)
{
    if (isVisible)
    {
        foreach(LIPLineGraphicsItem *item, mapFeatures)
            item->setVisible(true);
    }
    else
    {
        foreach(LIPLineGraphicsItem *item, mapFeatures)
            item->setVisible(false);
    }
}
