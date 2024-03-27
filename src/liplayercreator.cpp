#include "liplayercreator.h"
#include "QMessageBox"
LIPLayerCreator::LIPLayerCreator(LIPGeometryType type, QString name, QString nameGIS, LIPCoordinateSystem* system)
    :
      fileNameAsString{name},
      pointLayer{nullptr},
      lineLayer{nullptr},
      polyLayer{nullptr}

{
    mainName=nameGIS;
    GDALAllRegister();
    OGRRegisterAll();
    QByteArray ba = name.toLocal8Bit();
    const char *nameChar = ba.data();
    geomType=type;
    switch (geomType)
    {
    case LIPGeometryType::LIPPoint: //если создаем точечный слой
    {
        const char *pszDriverName = "ESRI Shapefile"; //toDO выбор драйвера пользователем
        GDALDriver *poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
        ds = poDriver->Create(nameChar, 0, 0, 0, GDT_Unknown, nullptr);
        layer = ds->CreateLayer(nameChar, system->getOGRSpatialRef(), wkbPoint, nullptr);
        if (layer == nullptr)
        {
            //cout << "Layer creation failed!" << endl;
        }
        else
        {
            pointLayer = new LIPPointLayer(layer, mainName, fileNameAsString, ds);
        }
    }
        break;
    case LIPGeometryType::LIPLineString: //если создаем линейный слой
    {
        const char *pszDriverName = "ESRI Shapefile"; //toDO выбор драйвера пользователем
        GDALDriver *poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
        ds = poDriver->Create(nameChar, 0, 0, 0, GDT_Unknown, NULL);
        layer = ds->CreateLayer(nameChar, system->getOGRSpatialRef(), wkbLineString, NULL);
        if (layer == NULL)
        {
            //cout << "Layer creation failed!" << endl;
        }
        else
        {
            lineLayer = new LIPLineLayer(layer, mainName, fileNameAsString, ds);
        }
    }
        break;
    case LIPGeometryType::LIPPolygon: //если создаем полигональный слой
    {
        const char *pszDriverName = "ESRI Shapefile"; //toDO выбор драйвера пользователем
        GDALDriver *poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
        ds = poDriver->Create(nameChar, 0, 0, 0, GDT_Unknown, NULL);
        layer = ds->CreateLayer(nameChar, system->getOGRSpatialRef(), wkbPolygon, NULL);
        if (layer == NULL)
        {
            //cout << "Layer creation failed!" << endl;
        }
        else
        {
            polyLayer = new LIPPolygonLayer(layer, mainName, fileNameAsString, ds);
        }
    }
    }





}

void LIPLayerCreator::createAttribute(LIPAttributeType type, QString name)
{
    bool typeFlag = false;
    QByteArray ba = name.toLocal8Bit();
    const char *nameChar = ba.data();
    switch (type)
    {
    case LIPAttributeType::INT32:
    {
            OGRFieldDefn poField(nameChar, OFTInteger); //создаем аттрибут
            poField.SetWidth(40);
            if (layer->CreateField(&poField) != OGRERR_NONE)
            {
                //cout << "ID field creation failed" << endl;
            }


    }
        break;
    case LIPAttributeType::String:
    {
            OGRFieldDefn poField(nameChar, OFTString); //создаем аттрибут
            poField.SetWidth(40);
            if (layer->CreateField(&poField) != OGRERR_NONE)
            {
                //cout << "ID field creation failed" << endl;
            }

    }
        break;
    case LIPAttributeType::Real:
    {
            OGRFieldDefn poField(nameChar, OFTReal); //создаем аттрибут
            poField.SetWidth(40);
            if (layer->CreateField(&poField) != OGRERR_NONE)
            {
                //cout << "ID field creation failed" << endl;
            }
        }

    }
//    switch (geomType)
//    {
//    case LIPGeometryType::LIPPoint: //если создаем точечный слой
//    {
//        pointLayer = new LIPPointLayer(layer, mainName, fileNameAsString, ds);
//    }
//        break;
//    case LIPGeometryType::LIPLineString: //если создаем линейный слой
//    {
//        lineLayer = new LIPLineLayer(layer, mainName, fileNameAsString, ds);
//    }
//        break;
//    case LIPGeometryType::LIPPolygon: //если создаем полигональный слой
//    {

//    }
//        polyLayer = new LIPPolygonLayer(layer, mainName, fileNameAsString, ds);
//    }


}

void LIPLayerCreator::setAttribute(LIPAttributeType type, QVariant attribute, QString name) //TODO переработать/удалить
{
//    bool typeFlag = false;
//    QByteArray ba = name.toLocal8Bit();
//    const char *nameChar = ba.data();
//    switch (type)
//    {
//    case LIPAttributeType::INT32:
//    {
//        attribute.toInt(&typeFlag);
//        if (typeFlag)
//        {
//            OGRFeature *Feature = OGRFeature::CreateFeature(layer->GetLayerDefn());
//            Feature->SetField(type, i);
//            Feature->SetField("Type", i);
//        }

//    }
//    case LIPAttributeType::String:
//    {
//        attribute.toString();
//        if (typeFlag)
//        {
//            OGRFieldDefn poField(nameChar, OFTString); //создаем аттрибут
//            poField.SetWidth(40);
//            if (layer->CreateField(&poField) != OGRERR_NONE)
//            {
//                //cout << "ID field creation failed" << endl;
//            }
//        }
//    }
//    case LIPAttributeType::Real:
//    {
//        attribute.toDouble(&typeFlag);
//        if (typeFlag)
//        {
//            OGRFieldDefn poField(nameChar, OFTReal); //создаем аттрибут
//            poField.SetWidth(40);
//            if (layer->CreateField(&poField) != OGRERR_NONE)
//            {
//                //cout << "ID field creation failed" << endl;
//            }
//        }
//    }
    //    }
}

LIPVectorLayer *LIPLayerCreator::returnLayer()
{
    switch (geomType)
    {
    case LIPGeometryType::LIPPoint: //если создаем точечный слой
    {
        return pointLayer;
        break;
    }

    case LIPGeometryType::LIPLineString: //если создаем линейный слой
    {
        return lineLayer;
        break;
    }

    case LIPGeometryType::LIPPolygon: //если создаем полигональный слой
    {
        return polyLayer;
        break;
    }

    }
}

LIPVectorLayer *LIPLayerCreator::createReprojectedLayer(LIPVectorLayer *inputLayer, LIPCoordinateSystem * targetCRS, QString fileName)
{
    if (inputLayer==nullptr)
        return nullptr;
    if (fileName.isEmpty())
        return nullptr;
    if (targetCRS==nullptr)
        return nullptr;
    LIPCoordinateSystem* sourceCRS = inputLayer->getCRS();
    OGRLayer* sourceLayer=inputLayer->getOGRLayer();
    if (sourceLayer==nullptr)
        return nullptr;
    if (sourceCRS==nullptr)
    {
        return nullptr;
    }
    CPLSetConfigOption("SHAPE_ENCODING", "UTF-8");

    QByteArray ba = fileName.toLocal8Bit();
    const char *nameChar = ba.data();

    OGRCoordinateTransformation* crTr = OGRCreateCoordinateTransformation(sourceCRS->getOGRSpatialRef(), targetCRS->getOGRSpatialRef());
    switch (LIPVectorTypeChecker::getType(inputLayer))
    {
    case LIPGeometryType::LIPPoint: //если создаем точечный слой
    {
        const char *pszDriverName = "ESRI Shapefile"; //toDO выбор драйвера пользователем
        GDALDriver *poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
        GDALDataset *ds = poDriver->Create(nameChar, 0, 0, 0, GDT_Unknown, nullptr);
        OGRLayer *layer = ds->CreateLayer(nameChar, targetCRS->getOGRSpatialRef(), wkbPoint, nullptr);
        if (layer == nullptr)
            return nullptr;

        sourceLayer->ResetReading();
        OGRFeature *feature;
        OGRFeatureDefn *featureDefn = sourceLayer->GetLayerDefn();
        for (int i = 0; i < featureDefn->GetFieldCount(); i++) {
            OGRFieldDefn *fieldDefn = featureDefn->GetFieldDefn(i);
            //newFeature->SetFrom(feature);
            layer->CreateField(fieldDefn);
            //newFeature->SetField(i, fieldDefn);
        }
        while ((feature = sourceLayer->GetNextFeature()) != nullptr) {


            // Создание нового объекта с измененными координатами
            OGRFeature *newFeature = OGRFeature::CreateFeature(featureDefn);
            newFeature->SetFrom(feature);

            OGRGeometry *poGeometry = feature->GetGeometryRef();
            for (int i=0;i<feature->GetGeomFieldCount();i++)
            {
                if (poGeometry != nullptr)
                {
                    OGRPoint *pointOGR=(OGRPoint*)poGeometry;
                    double x=pointOGR->getX();
                    double y=pointOGR->getY();
                    crTr->Transform(1, &x, &y);
                    qDebug()<<newFeature->SetGeometry(new OGRPoint(x, y));

                }
            }


            // Копирование атрибутов


            // Добавление нового объекта в слой
            qDebug()<<layer->CreateFeature(newFeature);
            layer->SyncToDisk();

            OGRFeature::DestroyFeature(feature);
            OGRFeature::DestroyFeature(newFeature);
        }

    }

    case LIPGeometryType::LIPLineString: //если создаем точечный слой
    {
        const char *pszDriverName = "ESRI Shapefile"; //toDO выбор драйвера пользователем
        GDALDriver *poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
        GDALDataset *ds = poDriver->Create(nameChar, 0, 0, 0, GDT_Unknown, nullptr);
        OGRLayer *layer = ds->CreateLayer(nameChar, targetCRS->getOGRSpatialRef(), wkbLineString, nullptr);
        if (layer == nullptr)
            return nullptr;

        sourceLayer->ResetReading();
        OGRFeature *feature;
        OGRFeatureDefn *featureDefn = sourceLayer->GetLayerDefn();
        for (int i = 0; i < featureDefn->GetFieldCount(); i++)
        {
            OGRFieldDefn *fieldDefn = featureDefn->GetFieldDefn(i);
            layer->CreateField(fieldDefn);
        }
        while ( (feature=sourceLayer->GetNextFeature()) != nullptr )
        {
            // Создание нового объекта с измененными координатами
            OGRFeature *newFeature = OGRFeature::CreateFeature(featureDefn);
            newFeature->SetFrom(feature);
            OGRGeometry *poGeometry = feature->GetGeometryRef();
            for (int i=0;i<feature->GetGeomFieldCount();i++)
            {
                qDebug()<<poGeometry->getGeometryType();
                if (poGeometry == nullptr)
                    continue;
                if (poGeometry->getGeometryType()==OGRwkbGeometryType::wkbLineString)
                {
                    OGRLineString *newLine = new OGRLineString;
                    OGRLineString *line = (OGRLineString *)poGeometry;
                    for (int pointIt = 0; pointIt < line->getNumPoints(); pointIt++)
                    {
                        double x = line->getX(pointIt);
                        double y = line->getY(pointIt);
                        crTr->Transform(1, &x, &y);
                        //qDebug()<<"x: "+QString::number(x) + " y: " +QString::number(y);
                        newLine->addPoint(new OGRPoint(x,y));
                    }
                    newFeature->SetGeometry(newLine);
                }
                else if (poGeometry->getGeometryType()==OGRwkbGeometryType::wkbMultiLineString)
                {
                    OGRMultiLineString *newLine = new OGRMultiLineString;
                    OGRMultiLineString *line = (OGRMultiLineString *)poGeometry;
                    for (int lineIt=0; lineIt<line->getNumGeometries(); lineIt++)
                    {
                        OGRLineString* oL=line->getGeometryRef(lineIt);
                        OGRLineString* nL = new OGRLineString;
                        for (int pointIt = 0; pointIt < oL->getNumPoints(); pointIt++)
                        {
                            double x = oL->getX(pointIt);
                            double y = oL->getY(pointIt);
                            crTr->Transform(1, &x, &y);
                            //qDebug()<<"x: "+QString::number(x) + " y: " +QString::number(y);
                            nL->addPoint(x,y);
                        }
                        newLine->addGeometry(nL);
                    }
                    newFeature->SetGeometry(newLine);
                }
            }
            // Добавление нового объекта в слой
            qDebug()<<layer->CreateFeature(newFeature);
            layer->SyncToDisk();

            OGRFeature::DestroyFeature(feature);
            OGRFeature::DestroyFeature(newFeature);
        }
        break;


    }
    case LIPGeometryType::LIPPolygon: //если создаем точечный слой
    {
        const char *pszDriverName = "ESRI Shapefile"; //toDO выбор драйвера пользователем
        GDALDriver *poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
        GDALDataset *ds = poDriver->Create(nameChar, 0, 0, 0, GDT_Unknown, nullptr);
        OGRLayer *layer = ds->CreateLayer(nameChar, targetCRS->getOGRSpatialRef(), wkbPolygon, nullptr);
        if (layer == nullptr)
            return nullptr;

        sourceLayer->ResetReading();
        OGRFeature *feature;
        OGRFeatureDefn *featureDefn = sourceLayer->GetLayerDefn();
        for (int i = 0; i < featureDefn->GetFieldCount(); i++) {
            OGRFieldDefn *fieldDefn = featureDefn->GetFieldDefn(i);
            //newFeature->SetFrom(feature);
            layer->CreateField(fieldDefn);
            //newFeature->SetField(i, fieldDefn);
        }
        while ((feature = sourceLayer->GetNextFeature()) != nullptr)
        {
            // Создание нового объекта с измененными координатами
            OGRFeature *newFeature = OGRFeature::CreateFeature(featureDefn);
            newFeature->SetFrom(feature);
            OGRGeometry *poGeometry = feature->GetGeometryRef();
            for (int i=0;i<feature->GetGeomFieldCount();i++)
            {
                if (poGeometry == nullptr)
                {
                    continue;
                }
                OGRwkbGeometryType type=poGeometry->getGeometryType();
                if (type==OGRwkbGeometryType::wkbPolygon)
                {
                    OGRPolygon *polygon = (OGRPolygon *)poGeometry;
                    OGRPolygon *newPolygon = new OGRPolygon;
                    OGRLinearRing* ring = polygon->getExteriorRing();
                    OGRLinearRing* newRing = new OGRLinearRing;
                    for (int pointIt = 0; pointIt < ring->getNumPoints(); pointIt++)
                    {
                        OGRPoint point;
                        ring->getPoint(pointIt, &point);
                        double x = point.getX();
                        double y = point.getY();
                        crTr->Transform(1, &x, &y);
                        newRing->addPoint(new OGRPoint(x,y));
                    }

                    newPolygon->addRing(newRing);
                    newFeature->SetGeometry(newPolygon);
                }
                else if (type==OGRwkbGeometryType::wkbMultiPolygon)
                {

                    OGRMultiPolygon *newPoly = new OGRMultiPolygon;
                    OGRMultiPolygon *poly = (OGRMultiPolygon *)poGeometry;
                    for (int polyIt = 0; polyIt < poly->getNumGeometries(); polyIt++)
                    {

                        auto polygon=poly->getGeometryRef(polyIt);
                        //qDebug()<<polygon->getExteriorRing();
                        OGRLinearRing* ring = polygon->getExteriorRing();
                        OGRLinearRing* newRing = new OGRLinearRing;
                        for (int pointIt = 0; pointIt < ring->getNumPoints(); pointIt++)
                        {
                            OGRPoint point;
                            ring->getPoint(pointIt, &point);
                            double x = point.getX();
                            double y = point.getY();
                            crTr->Transform(1, &x, &y);
                            newRing->addPoint(new OGRPoint(x,y));
                        }
                        newPoly->addGeometry(newRing);

                    }
                    newFeature->SetGeometry(newPoly);

                }
            }


            // Копирование атрибутов


            // Добавление нового объекта в слой
            qDebug()<<layer->CreateFeature(newFeature);
            layer->SyncToDisk();

            OGRFeature::DestroyFeature(feature);
            OGRFeature::DestroyFeature(newFeature);
        }
        break;



    }
    case LIPGeometryType::LIPUnknown:
        {
            break;
        }
    }





}

OGRLayer* LIPLayerCreator::returnOGRLayer()
{
    return layer;
}
