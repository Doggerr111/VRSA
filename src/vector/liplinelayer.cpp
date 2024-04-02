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
    mStyle=LIPVectorStyle::createDefaultVectorStyle(LIPGeometryType::LIPLineString);
    LIPWidgetManager::getInstance().getMainWindow()->addLayer(this);
}

LIPLineLayer::~LIPLineLayer()
{
    for(int i=0; i<mapFeatures.size(); i++)
    {
        delete mapFeatures.at(i);
    }
    for(QVector<LIPPoint*> vec: coordinates)
    {
        for(LIPPoint* point: vec)
        {
            delete point;
        }
        vec.clear();
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
    layer = shpDS->GetLayer(0);
}

QString LIPLineLayer::returnGISName()
{
    return GISName;
}


QVector<QVector<LIPPoint*>>  LIPLineLayer::returnCords()
{
    if (layer==nullptr)
    {
        return QVector<QVector<LIPPoint*>>();
    }
//    for(QVector<LIPPoint*> points: coordinates)
//    {
//        for (LIPPoint *point: points)
//        {
//            delete point;
//            point=nullptr;
//        }
//        points.clear();
//    }
//    coordinates.clear();

        layer->GetName();
        OGRFeature *shpFeature;
        layer->ResetReading();
        int counter=0;
        while ((shpFeature = layer->GetNextFeature()) != NULL)
        {
            counter++;
            if (counter-1<coordinates.size())
                continue;
            OGRGeometry *poGeometry = shpFeature->GetGeometryRef();
            if (poGeometry==nullptr)
                continue;
            OGRwkbGeometryType type=poGeometry->getGeometryType();
            if (type==OGRwkbGeometryType::wkbLineString)
            {
                for (int i=0;i<shpFeature->GetGeomFieldCount();i++)
                {

                    OGRwkbGeometryType type=poGeometry->getGeometryType();
                    qDebug()<<"geomLineTpe is:";
                    qDebug()<<type;
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


                    //qDebug()<<vect;
                    coordinates.append(vect);
                }

            }

            if (type==OGRwkbGeometryType::wkbMultiLineString)
            {
                for (int i=0;i<shpFeature->GetGeomFieldCount();i++)
                {



                    OGRMultiLineString *lines = (OGRMultiLineString *)poGeometry;
                    for (int i=0; i<lines->getNumGeometries(); i++)
                    {
                        QVector<LIPPoint*> vect;
                        OGRLineString *line = lines->getGeometryRef(i);
                        for (int i = 0; i < line->getNumPoints(); i++)
                        {
                            LIPPoint *point = new LIPPoint();
                            point->setX(line->getX(i));
                            point->setY(line->getY(i));
                            vect.append(point);
                            //qDebug()<<"wkbLineString %d: x=%g y=%g z=%g\n", i, line->getX(i), line->getY(i), line->getZ(i);
                        }
                        //qDebug()<<vect;
                        coordinates.append(vect);
                    }
                }

            }

            //qDebug()<<"line cords";
            //qDebug()<<coordinates.size();

            OGRFeature::DestroyFeature(shpFeature);


        }
        return coordinates;

}

void LIPLineLayer::update()
{
//    for(int i=0; i<mapFeatures.size(); i++)
//    {
//        delete mapFeatures.at(i);
//        mapFeatures[i]=nullptr;
//    }

//    for(QVector<LIPPoint*> points: coordinates)
//    {
//        for (LIPPoint *point: points)
//        {
//            delete point;
//            point=nullptr;
//        }
//        points.clear();
//    }

//    mapFeatures.clear();
//    coordinates.clear();

    //setMapFeatures();
    emit needRepaint();
}

void LIPLineLayer::setStyle(LIPVectorStyle *style)
{
    mStyle=style; //field of vectorLayer
    mStyle->setGeomType(LIPGeometryType::LIPLineString);
    foreach(LIPLineGraphicsItem *item, mapFeatures)
    {
        item->setVectorStyle(mStyle); //задаем стиль для каждого обьекта слоя
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

    //qDebug()<<vect.size();
    for (int i=0; i<vect.size(); i++)
    {
        if (i<mapFeatures.size())
            continue;
        LIPLineGraphicsItem *el = new LIPLineGraphicsItem;
        el->setIndex(i);
        connect(el, &LIPGraphicsItem::clicked, this, &LIPLineLayer::itemClicked);
        el->setVectorStyle(mStyle);
        el->setPoints(vect.at(i));
        el->setScaleFactor(mScaleFactor);

        mapFeatures.append(el);

    }

}

void LIPLineLayer::itemClicked(int ind)
{
    if (LIPProject::getInstance().isSelectingFeatures())
    {
        if (mSelectedFeatureIndex!=-1)
            mapFeatures.at(mSelectedFeatureIndex)->deselect();
        mapFeatures.at(ind)->select();
        mSelectedFeatureIndex=ind;
        LIPFeatureAttributesForm* form = new LIPFeatureAttributesForm;
        form->setFeature(layer, ind);
        form->exec();
        delete form;
    }
}

void LIPLineLayer::deselectItems()
{
    if (mSelectedFeatureIndex!=-1)
        mapFeatures.at(mSelectedFeatureIndex)->deselect();
}

QVector<LIPLineGraphicsItem *> LIPLineLayer::returnMapFeatures()
{
    return mapFeatures;
}


void LIPLineLayer::addFeature(QVector<QPointF> coords, QVector<LIPAttribute> attrs)
{
    OGRFeature *newFeature = OGRFeature::CreateFeature(layer->GetLayerDefn());
    //OGRPolygon *polygon = new OGRPolygon();
    OGRLineString *lineString = new OGRLineString();
    //LIPLineGraphicsItem *line = new LIPLineGraphicsItem();
    QVector<LIPPoint*> pointPtrs;
    for (int i = 0; i < coords.size(); i++)
    {
        LIPPoint* p = new LIPPoint;
        p->setX(coords[i].x());
        p->setY(coords[i].y());
        pointPtrs.append(p);
    }
    //line->setPoints(pointPtrs);
    //mapFeatures.append(line);
    for (int i=0; i<coords.size(); i++)
    {
        lineString->addPoint(coords.at(i).x(), coords.at(i).y());
    }




    for (int i=0; i<attrs.size(); i++)
    {
        QString fieldName = attrs.at(i).getName();
        QByteArray fieldNameBa = fieldName.toLocal8Bit();
        const char *fieldNameChar = fieldNameBa.data();
        switch (attrs.at(i).getType())
        {
        case LIPAttributeType::INT32:
            newFeature->SetField(fieldNameChar, attrs.at(i).getValue().toInt());
            break;
        case LIPAttributeType::Real:
            newFeature->SetField(fieldNameChar, attrs.at(i).getValue().toDouble());
            break;
        case LIPAttributeType::String:
            const char *fieldValueChar=attrs.at(i).getValue().toString().toLocal8Bit().data();
            newFeature->SetField(fieldNameChar, fieldValueChar);
            break;
        }
    }

    newFeature->SetGeometry(lineString);
    newFeature->SetFID(layer->GetFeatureCount());

    // Добавление объекта к слою
    //OGRErr er1 = layer->StartTransaction();
    OGRErr er = layer->CreateFeature(newFeature);
    if (er != OGRERR_NONE)
        LIPWidgetManager::getInstance().showMessage("Ошибка создания объекта", 1000, Error);
    layer->GetLayerDefn()->SetGeomType(wkbPolygon);
    layer->SetSpatialFilter(nullptr);
    //layer->CommitTransaction();
    layer->SyncToDisk();
    //setMapFeatures();
    //GDALClose(layer);

    OGRFeature::DestroyFeature(newFeature);
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


void LIPLineLayer::setZValue(int zValue)
{
    for(auto feature: mapFeatures)
    {
        feature->setZValue(zValue);
    }
}



