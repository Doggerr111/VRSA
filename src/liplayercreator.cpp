#include "liplayercreator.h"

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
        layer = ds->CreateLayer(nameChar, system, wkbPoint, nullptr);
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
        layer = ds->CreateLayer(nameChar, system, wkbLineString, NULL);
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
        layer = ds->CreateLayer(nameChar, system, wkbPolygon, NULL);
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
    }
        break;
    case LIPGeometryType::LIPLineString: //если создаем линейный слой
    {
        return lineLayer;
    }
        break;
    case LIPGeometryType::LIPPolygon: //если создаем полигональный слой
    {

    }
        return polyLayer;
    }
}

OGRLayer* LIPLayerCreator::returnOGRLayer()
{
    return layer;
}
