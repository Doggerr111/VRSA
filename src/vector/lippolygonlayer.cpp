#include "lippolygonlayer.h"
#include <QDebug>
#include "mainwindow.h"
LIPPolygonLayer::LIPPolygonLayer(OGRLayer *l, QString name, QString path, GDALDataset *ds)
    : LIPVectorLayer(l, path, ds),
      //layer{l},
      GISName{name},
      fileName{path}
{

    mStyle=LIPVectorStyle::createDefaultVectorStyle(LIPGeometryType::LIPPolygon);
    LIPWidgetManager::getInstance().getMainWindow()->addLayer(this);

    qDebug()<<"LAYER CRS IS:";
    char *pszWKT = NULL;
    //    qDebug()<<l->GetSpatialRef()->exportToWkt(&pszWKT);
    //    qDebug()<<pszWKT;
    //    qDebug()<<l->GetSpatialRef()->GetAuthorityCode(nullptr);//jijlk

}

LIPPolygonLayer::~LIPPolygonLayer()
{
    qDeleteAll(mapFeatures);
    mapFeatures.clear();
    //qDeleteAll(coordinates);

    for(QVector<LIPPoint*> vec: coordinates)
    {
        qDeleteAll(vec);
    }
    coordinates.clear();

    //    for(int i=0; i<mapFeatures.size(); i++)
    //    {
    //        delete mapFeatures.at(i);
    //        mapFeatures[i] = nullptr;
    //    }
    //    for(QVector<LIPPoint*> vec: coordinates)
    //    {
    //        for(LIPPoint* point: vec)
    //        {
    //            delete point;
    //            point=nullptr;
    //        }
    //        //vec.clear();

    //    }
    //    mapFeatures.clear();
    //    coordinates.clear();




    //    qDeleteAll(mapFeatures);
    //    qDeleteAll(coordinates);
}

QString LIPPolygonLayer::returnGISName()
{

    return GISName;

}

QVector<QVector<LIPPoint *> > LIPPolygonLayer::returnCords()
{
    if (layer==nullptr)
        return QVector<QVector<LIPPoint*>>();

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
        if (poGeometry == nullptr)
            continue;
        for (int i=0;i<shpFeature->GetGeomFieldCount();i++)
        {
            OGRwkbGeometryType type=poGeometry->getGeometryType();
            if (type==OGRwkbGeometryType::wkbPolygon)
            {
                QVector<LIPPoint*> vect;
                OGRPolygon *polygon = (OGRPolygon *)poGeometry;

                //qDebug()<<polygon->getExteriorRing();
                OGRLinearRing* ring = polygon->getExteriorRing();

                for (int i = 0; i < ring->getNumPoints(); i++)
                {
                    OGRPoint point;
                    LIPPoint *pointN = new LIPPoint();
                    ring->getPoint(i, &point);
                    double x = point.getX();
                    double y = point.getY();
                    pointN->setX(x);
                    pointN->setY(y);
                    vect.append(pointN);
                }

                coordinates.append(vect);
            }
            else if (type==OGRwkbGeometryType::wkbMultiPolygon)
            {


                OGRMultiPolygon *polygons = (OGRMultiPolygon *)poGeometry;
                for (int i=0; i<polygons->getNumGeometries(); i++)
                {
                    QVector<LIPPoint*> vect;
                    auto polygon=polygons->getGeometryRef(i);
                    //qDebug()<<polygon->getExteriorRing();
                    OGRLinearRing* ring = polygon->getExteriorRing();

                    for (int i = 0; i < ring->getNumPoints(); i++)
                    {
                        OGRPoint point;
                        LIPPoint *pointN = new LIPPoint();
                        ring->getPoint(i, &point);
                        double x = point.getX();
                        double y = point.getY();
                        pointN->setX(x);
                        pointN->setY(y);
                        vect.append(pointN);
                    }

                    coordinates.append(vect);
                }


            }

        }
        OGRFeature::DestroyFeature(shpFeature);
    }
    qDebug()<<"polygon coords:";
    qDebug()<<coordinates.size();
    return coordinates;


}

void LIPPolygonLayer::setFileName(QString path)
{
    fileName=path;
}

QString LIPPolygonLayer::getFileName()
{
    return fileName;
}

void LIPPolygonLayer::setMapFeatures()
{
    QVector<QVector<LIPPoint*>> vect = returnCords();
    for (int i=0; i<vect.size(); i++)
    {
        if (i<mapFeatures.size())
            continue;
        LIPPolygonGraphicsItem *el = new LIPPolygonGraphicsItem();
        el->setIndex(i);
        connect(el, &LIPGraphicsItem::clicked, this, &LIPPolygonLayer::itemClicked);
        el->setVectorStyle(mStyle);
        el->setPoints(vect.at(i));
        el->setScaleFactor(mScaleFactor);
        mapFeatures.append(el);
    }

}

void LIPPolygonLayer::itemClicked(int ind)
{
    if (LIPProject::getInstance().isSelectingFeatures())
    {
        if (mSelectedFeatureIndex!=-1)
            mapFeatures.at(mSelectedFeatureIndex)->deselect();
        mapFeatures.at(ind)->select();
        mSelectedFeatureIndex=ind;
        //mapFeatures.at(ind)->update();
        LIPFeatureAttributesForm* form = new LIPFeatureAttributesForm;
        form->setFeature(layer, ind);
        form->exec();
        delete form;
    }
}

void LIPPolygonLayer::deselectItems()
{
    if (mSelectedFeatureIndex!=-1)
        mapFeatures.at(mSelectedFeatureIndex)->deselect();
}

void LIPPolygonLayer::setVisible(bool isVisible)
{
    if (isVisible)
    {
        foreach(LIPPolygonGraphicsItem *item, mapFeatures)
            item->setVisible(true);
    }
    else
    {
        foreach(LIPPolygonGraphicsItem *item, mapFeatures)
            item->setVisible(false);
    }
}

QVector<LIPPolygonGraphicsItem*> LIPPolygonLayer::returnMapFeatures()
{
    return mapFeatures;
}

void LIPPolygonLayer::addFeature(QVector<QPointF> coords, QVector<LIPAttribute> attrs)
{
    OGRFeature *newFeature = OGRFeature::CreateFeature(layer->GetLayerDefn());
    OGRwkbGeometryType type= layer->GetLayerDefn()->GetGeomType();
    if (type==OGRwkbGeometryType::wkbPolygon)
    {
        OGRPolygon *polygon = new OGRPolygon();
        OGRLinearRing ring;// = new OGRLinearRing;// = polygon->getExteriorRing();
        //layer->StartTransaction();
        //LIPPolygonGraphicsItem *el = new LIPPolygonGraphicsItem();
        QVector<LIPPoint*> pointPtrs;
        for (int i = 0; i < coords.size(); i++)
        {
            LIPPoint* p = new LIPPoint;
            p->setX(coords[i].x());
            p->setY(coords[i].y());
            pointPtrs.append(p);
        }
        //el->setPoints(pointPtrs);
        //mapFeatures.append(el);
        for (int i=0; i<coords.size(); i++)
        {
            ring.addPoint(coords.at(i).x(), coords.at(i).y());
        }
        ring.closeRings(); //ОБЯЗАТЕЛЬНО ПРИ СОЗДАНИИ ПОЛИГОНАЛЬНОГО ОБЬЕКТА!! ИНАЧЕ ОШИБКИ ПРИ РАБОТЕ СО СЛОЕМ
        OGRErr R=polygon->addRingDirectly(&ring);


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

        newFeature->SetGeometry(polygon);
        newFeature->SetFID(layer->GetFeatureCount());

        // Добавление объекта к слою
        //OGRErr er1 = layer->StartTransaction();
        OGRErr er = layer->CreateFeature(newFeature);
        //layer->SetSpatialFilter(nullptr);
        //er1= layer->CommitTransaction();
    }
    if (type==OGRwkbGeometryType::wkbMultiPolygon)
    {
        OGRMultiPolygon *polygons = new OGRMultiPolygon();
        OGRPolygon *polygon = new OGRPolygon();
        OGRLinearRing ring;// = new OGRLinearRing;// = polygon->getExteriorRing();
        //layer->StartTransaction();
        LIPPolygonGraphicsItem *el = new LIPPolygonGraphicsItem();
        QVector<LIPPoint*> pointPtrs;
        for (int i = 0; i < coords.size(); i++)
        {
            LIPPoint* p = new LIPPoint;
            p->setX(coords[i].x());
            p->setY(coords[i].y());
            pointPtrs.append(p);
        }
        el->setPoints(pointPtrs);
        mapFeatures.append(el);
        for (int i=0; i<coords.size(); i++)
        {
            ring.addPoint(coords.at(i).x(), coords.at(i).y());
        }
        ring.closeRings(); //ОБЯЗАТЕЛЬНО ПРИ СОЗДАНИИ ПОЛИГОНАЛЬНОГО ОБЬЕКТА!! ИНАЧЕ ОШИБКИ ПРИ РАБОТЕ СО СЛОЕМ
        OGRErr R=polygon->addRingDirectly(&ring);
        polygons->addGeometry(polygon);

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

        newFeature->SetGeometry(polygons);
        //newFeature->SetFID(layer->GetFeatureCount());

        // Добавление объекта к слою
        //OGRErr er1 = layer->StartTransaction();
        OGRErr er = layer->CreateFeature(newFeature);
        //layer->SetSpatialFilter(nullptr);
        //er1= layer->CommitTransaction();
    }
    layer->SyncToDisk();
    OGRFeature::DestroyFeature(newFeature);
}

void LIPPolygonLayer::setStyle(LIPVectorStyle *style)
{
    mStyle=style; //field of vectorLayer
    mStyle->setGeomType(LIPGeometryType::LIPPolygon);
    foreach(LIPPolygonGraphicsItem *item, mapFeatures)
    {
        item->setVectorStyle(style); //задаем стиль для каждого обьекта слоя
    }
}

void LIPPolygonLayer::setSceneScaleFactor(double factor)
{
    mScaleFactor=factor;
    foreach(LIPPolygonGraphicsItem* item, mapFeatures)
    {
        item->setScaleFactor(mScaleFactor);
    }
}



void LIPPolygonLayer::selectFeature(int index)
{
    mapFeatures.at(index)->select();
    mapFeatures.at(index)->update();
}


void LIPPolygonLayer::setZValue(int zValue)
{
    for(auto feature: mapFeatures)
    {
        feature->setZValue(zValue);
    }
}


void LIPPolygonLayer::update()
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
//    }

//    mapFeatures.clear();
//    coordinates.clear();

    //setMapFeatures();
    emit needRepaint();

}


