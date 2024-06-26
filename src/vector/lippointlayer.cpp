#include "lippointlayer.h"
#include "mainwindow.h"
LIPPointLayer::LIPPointLayer(OGRLayer *l, QString name, QString fileName, GDALDataset *dataset)
    : LIPVectorLayer(l, fileName, dataset),
      //layer{l},
      GISName(name)
{
    mStyle=LIPVectorStyle::createDefaultVectorStyle(LIPGeometryType::LIPPoint);
    LIPWidgetManager::getInstance().getMainWindow()->addLayer(this);
}

LIPPointLayer::~LIPPointLayer()
{
    for(int i=0; i<mapFeatures.size(); i++)
    {
        //LIPWidgetManager::getInstance().getScene()->removeItem(mapFeatures.at(i));
        delete mapFeatures.at(i);
    }

    foreach(LIPPoint* point, coordinates)
    {
        delete point;
    }

    mapFeatures.clear();
    coordinates.clear();
    //delete mStyle;
}

QString LIPPointLayer::returnGISName()
{
    return GISName;
}

bool LIPPointLayer::reproject(LIPCoordinateSystem *targetCRS)
{


    if (layer==nullptr || targetCRS==nullptr)
        return false;
    OGRSpatialReference *sourceCRS = layer->GetSpatialRef();
    /*OGRLayer* newLayer=*/dS->CreateLayer("layerRep", targetCRS->getOGRSpatialRef(), wkbPoint);

    if (sourceCRS==nullptr)
    {

        LIPWidgetManager::getInstance().showMessage("Перепроицирование невозможно так как для слоя не указана система координат,"
                                                    "Необходимо сначала задать систему координат", 5000, messageStatus::Error);
        return false;
    }
    OGRCoordinateTransformation* crTr = OGRCreateCoordinateTransformation(mCRS->getOGRSpatialRef(), targetCRS->getOGRSpatialRef());
    OGRFeature *shpFeature;
    layer->ResetReading();

    while ((shpFeature = layer->GetNextFeature()) != nullptr)
    {

        OGRGeometry *poGeometry = shpFeature->GetGeometryRef();
        //int count = shpFeature->GetGeomFieldCount();
        for (int i=0;i<shpFeature->GetGeomFieldCount();i++)
        {
            if (poGeometry != nullptr)
            {
                OGRPoint *pointOGR=(OGRPoint*)poGeometry;

                double x=pointOGR->getX();
                double y=pointOGR->getY();
                //qDebug()<<x;

                crTr->Transform(1, &x, &y);
                //qDebug()<<x;
                pointOGR->setX(x);
                pointOGR->setY(y);
                //qDebug()<<shpFeature->SetGeometry(pointOGR);
                auto er = OGR_L_SetFeature(layer,shpFeature);
                Q_UNUSED(er);

            }
        }
    }
    mCRS=targetCRS;
    OGRFeature::DestroyFeature(shpFeature);
    //layer->SyncToDisk();
    dS->StartTransaction();
    //GDALSetProjection(dS, targetCRS->ExportToWkt());
    dS->SetSpatialRef(targetCRS->getOGRSpatialRef());
    dS->CommitTransaction();
    //TODO Для изменения слоя нужно создать новый OGRLayer в который скопировать все старые параметры

    update();
    return true;

}

bool LIPPointLayer::reproject(LIPCoordinateSystem *sourceCRS, LIPCoordinateSystem *targetCRS)
{
    Q_UNUSED(sourceCRS);
    Q_UNUSED(targetCRS);
    return true;
}

bool LIPPointLayer::reproject(LIPCoordinateSystem *targetCRS, QString fileName)
{
    Q_UNUSED(fileName);
    Q_UNUSED(targetCRS);
    return false;

    //    if (layer==nullptr)
    //    {
    //        LIPWidgetManager::getInstance().showMessage("Для данного слоя перепроицирование невозможно", 2000, messageStatus::Error);
    //        return false;
    //    }
    //    if (mCRS==nullptr)
    //    {

    //        LIPWidgetManager::getInstance().showMessage("Перепроицирование невозможно так как для слоя не указана система координат,"
    //                                              "Необходимо сначала задать систему координат", 5000, messageStatus::Error);
    //        return false;
    //    }
    //    //auto *driver = GDALDriverManager::GetDriverByName("ESRI Shapefile");
    //    auto *driver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("ESRI Shapefile");
    //    GDALDataset *targetDataset = driver->CreateDataSource(targetFile, nullptr);


    //    OGRCoordinateTransformation* crTr = OGRCreateCoordinateTransformation(mCRS, targetCRS);
    //    OGRFeature *shpFeature;
    //    layer->ResetReading();

    //    while ((shpFeature = layer->GetNextFeature()) != nullptr)
    //    {

    //        OGRGeometry *poGeometry = shpFeature->GetGeometryRef();
    //        int count = shpFeature->GetGeomFieldCount();
    //        for (int i=0;i<shpFeature->GetGeomFieldCount();i++)
    //        {
    //            if (poGeometry != nullptr)
    //            {
    //                OGRPoint *pointOGR=(OGRPoint*)poGeometry;

    //                double x=pointOGR->getX();
    //                double y=pointOGR->getY();
    //                //qDebug()<<x;

    //                crTr->Transform(1, &x, &y);
    //                //qDebug()<<x;
    //                pointOGR->setX(x);
    //                pointOGR->setY(y);
    //                //qDebug()<<shpFeature->SetGeometry(pointOGR);
    //                OGR_L_SetFeature(layer,shpFeature);

    //            }
    //        }
    //    }
    //    mCRS=targetCRS;
    //    OGRFeature::DestroyFeature(shpFeature);
    //    //layer->SyncToDisk();
    //    dS->StartTransaction();
    //    //GDALSetProjection(dS, targetCRS->ExportToWkt());
    //    dS->SetSpatialRef(targetCRS);
    //    dS->CommitTransaction();
    //    //TODO Для изменения слоя нужно создать новый OGRLayer в который скопировать все старые параметры

    //    update();
    //    return true;
}
void LIPPointLayer::flyReprojection()
{

    update();


}



void LIPPointLayer::update()
{
    //    for(int i=0; i<mapFeatures.size(); i++)
    //    {
    //        delete mapFeatures.at(i);
    //    }

    //    foreach(LIPPoint* point, coordinates)
    //    {
    //        delete point;
    //    }

    //    mapFeatures.clear();
    //    coordinates.clear();
    //setMapFeatures();
    emit needRepaint();

}

QVector<LIPPoint *> LIPPointLayer::returnCords()
{
    //    for(LIPPoint* point: coordinates)
    //    {
    //        delete point;
    //    }
    //    coordinates.clear();


    if (layer==nullptr)
        return QVector<LIPPoint*>();

    OGRFeature *shpFeature;
    layer->ResetReading();
    OGRSpatialReference *targetCRS =  LIPProject::getInstance().getProjectCRS()->getOGRSpatialRef();
    //auto sou = layer->GetSpatialRef();
    OGRCoordinateTransformation* crTr = OGRCreateCoordinateTransformation(layer->GetSpatialRef(), targetCRS);
//    if (targetCRS->IsSameGeogCS(layer->GetSpatialRef()))
//        qDebug()<<"coordinates are same";
//    qDebug()<<targetCRS->GetAuthorityCode(nullptr);
//    qDebug()<<layer->GetSpatialRef()->GetAuthorityCode(nullptr);


    int counter=0;
    while ((shpFeature = layer->GetNextFeature()) != NULL)
    {
        counter++;
        if (counter-1<coordinates.size())
            continue;
        OGRGeometry *poGeometry = shpFeature->GetGeometryRef();
        if (poGeometry==nullptr)
            continue;
        for (int i=0;i<shpFeature->GetGeomFieldCount();i++)
        {
            OGRPoint *pointOGR=(OGRPoint*)poGeometry;
            LIPPoint *point = new LIPPoint();
            double x=pointOGR->getX();
            double y=pointOGR->getY();
            crTr->Transform(1, &x, &y);
            point->setX(x);
            point->setY(y);
            delete pointOGR;
            coordinates.append(point);

        }

    }
    OGRFeature::DestroyFeature(shpFeature);
    return coordinates;


}

void LIPPointLayer::setMapFeatures()
{
    returnCords();
    for (int i=0; i<coordinates.size(); i++)
    {
        if (i<mapFeatures.size())
            continue;
        LIPPointGraphicsItem* item = new LIPPointGraphicsItem;
        item->setIndex(i);
        connect(item, &LIPGraphicsItem::clicked, this, &LIPPointLayer::itemClicked);
        item->setVectorStyle(mStyle);
        //        LIPPoint *point = new LIPPoint();
        //        point->setX(coordinates.at(i)->x());


        //        point->setY(coordinates.at(i)->y());
        item->setPoint(coordinates.at(i));
        item->setScaleFactor(mScaleFactor);

        mapFeatures.append(item);
    }
}
void LIPPointLayer::itemClicked(int ind)
{
    if (LIPProject::getInstance().isSelectingFeatures())
    {
        if (LIPProject::getInstance().getActiveLayer() != this)
            return;
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

void LIPPointLayer::deselectItems()
{
    if (mSelectedFeatureIndex!=-1)
        mapFeatures.at(mSelectedFeatureIndex)->deselect();
}

QVector<LIPPointGraphicsItem *> LIPPointLayer::returnMapFeatures()
{
    return mapFeatures;
}

void LIPPointLayer::setFileName(QString path)
{
    fileName=path;
}

QString LIPPointLayer::getFileName()
{
    return fileName;
}





void LIPPointLayer::addFeature(QVector<QPointF> coords, QVector<LIPAttribute> attrs)
{
    OGRFeature *newFeature = OGRFeature::CreateFeature(layer->GetLayerDefn());
    //OGRwkbGeometryType t= layer->GetLayerDefn()->GetGeomType();
    OGRPoint featurePoint;

    //for (int i=0; i<coords.size(); i++)
    //{
    //не проходим по всему вектроу, так как точка только 1 (мультиточки не поддерживаются)
    featurePoint.setX(coords.at(0).x());
    featurePoint.setY(coords.at(0).y());

    //    LIPPointGraphicsItem* item = new LIPPointGraphicsItem;
    //    LIPPoint *point = new LIPPoint();
    //    point->setX(coords.at(0).x());
    //    point->setY(coords.at(0).y());
    //    item->setPoint(point);
    //    item->setIndex(mapFeatures.size());
    //    mapFeatures.append(item);

    //}
    newFeature->SetGeometry(&featurePoint);
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

    newFeature->SetFID(layer->GetFeatureCount());

    // Добавление объекта к слою
    OGRErr er1 = layer->StartTransaction();
    OGRErr er = layer->CreateFeature(newFeature);

    layer->GetLayerDefn()->SetGeomType(wkbPolygon);
    layer->SetSpatialFilter(nullptr);
    er1= layer->CommitTransaction();
    if (er1!=OGRERR_NONE || er!=OGRERR_NONE)
        LIPWidgetManager::getInstance().showMessage("Ошибка создания объекта", 1000, Error);
    layer->SyncToDisk();
    //GDALClose(layer);
    //setMapFeatures();
    OGRFeature::DestroyFeature(newFeature);
}

void LIPPointLayer::setStyle(LIPVectorStyle *style)
{
    mStyle=style; //field of vectorLayer
    mStyle->setGeomType(LIPGeometryType::LIPPoint);
    foreach(LIPPointGraphicsItem *item, mapFeatures)
    {
        item->setVectorStyle(style); //задаем стиль для каждого обьекта слоя
    }
}



void LIPPointLayer::setSceneScaleFactor(double factor)
{
    mScaleFactor=factor;
    foreach(LIPPointGraphicsItem* item, mapFeatures)
    {
        item->setScaleFactor(mScaleFactor);
    }
}


void LIPPointLayer::setVisible(bool isVisible)
{
    if (isVisible)
    {
        foreach(LIPPointGraphicsItem *item, mapFeatures)
            item->setVisible(true);
    }
    else
    {
        foreach(LIPPointGraphicsItem *item, mapFeatures)
            item->setVisible(false);
    }
}


void LIPPointLayer::setZValue(int zValue)
{
    for(auto feature: mapFeatures)
    {
        feature->setZValue(zValue);
    }
}





