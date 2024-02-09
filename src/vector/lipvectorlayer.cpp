#include "lipvectorlayer.h"
#include "QDebug"
#include "mainwindow.h"
#include "lippointlayer.h"
#include "liplinelayer.h"
#include "lippolygonlayer.h"
LIPVectorLayer::LIPVectorLayer(OGRLayer *l, QString path, GDALDataset *dataset)
    : layer{l},
      dS{dataset},
      fileName{path},
      mScaleFactor{1.0},
      mStyle{nullptr},
      mZValue{0}
      //mCRS{nullptr}
{
    //получаем текущий масштаб виджета, чтобы при загрузке векторных данных (например) они сразу отображались в нужном масштабе

    mScaleFactor = LIPWidgetManager::getInstance().getMainWindow()->findChild<QGraphicsView*>("graphicsView")->transform().m11();

    if (l!=nullptr)
    {
        //qDebug()<<l->GetSpatialRef()->GetEPSGGeogCS();
        mCRS=static_cast<LIPCoordinateSystem*>(l->GetSpatialRef());
    }
}


QString LIPVectorLayer::returnGISName()
{

}

QString LIPVectorLayer::returnFileName()
{
    return fileName;
}

LIPVectorLayer::~LIPVectorLayer()
{
    mStyle=nullptr;
    delete mStyle;
    dS=nullptr;
    GDALClose(dS);
    layer=nullptr;
    delete layer;
}

void LIPVectorLayer::addFeature(QVector<QPointF> coords, QVector<LIPAttribute> attrs)
{

}

void LIPVectorLayer::setVisible(bool)
{

}

bool LIPVectorLayer::reproject(LIPCoordinateSystem *targetCRS)
{
    return true;
}

bool LIPVectorLayer::reproject(LIPCoordinateSystem *sourceCRS, LIPCoordinateSystem *targetCRS)
{

}

void LIPVectorLayer::update()
{
    emit needRepaint();
}
/////////////
std::map<int, QVector<LIPAttribute>> LIPVectorLayer::getAttributes()
{
    return LIPVectorReader::readAttributes(layer);
}

bool LIPVectorLayer::setCoordinateSystem(LIPCoordinateSystem *targetCRS)
{
    if (targetCRS==nullptr)
        return false;

    //dS->SetSpatialRef(targetCRS);
    OGRDataSource* ds;
    //ds->SetSpatialRef(targetCRS);
    //char** pszWKT;
    //targetCRS->exportToWkt(pszWKT);
    //dS->SetProjection(targetCRS->getProj());
    //layer->GetSpatialRef()->SetProjection(targetCRS->getProj());
    qDebug()<<layer->GetLayerDefn()->OGRFeatureDefn::GetGeomFieldDefn(0)->GetSpatialRef();
    layer->GetLayerDefn()->OGRFeatureDefn::GetGeomFieldDefn(0)->SetSpatialRef(targetCRS);
    qDebug()<<layer->GetLayerDefn()->OGRFeatureDefn::GetGeomFieldDefn(0)->GetSpatialRef();
    layer->SyncToDisk();
//    const char *originalString = layer->GetName();
//    const char *stringToAdd = "_rep";
//    size_t originalStringLength = strlen(originalString);
//    size_t stringToAddLength = strlen(stringToAdd);
//    char *resultString = (char *)malloc((originalStringLength + stringToAddLength + 1) * sizeof(char));
//    strcpy(resultString, originalString);
//    strcat(resultString, stringToAdd);
//    //free(resultString);

//    // так как нет функции, меняющей систему координат OGRLayer, придется создать новый слой и скопировать в него все объекты
//    OGRLayer* newLayer = dS->CreateLayer(resultString, targetCRS, layer->GetGeomType(), nullptr); //слой с новой СК
//    QString oldName=layer->GetName();
//    // копирование атрибутов
//    OGRFeatureDefn* featureDefn = layer->GetLayerDefn();
//    for (int i = 0; i < featureDefn->GetFieldCount(); i++)
//    {
//        OGRFieldDefn* fieldDefn = featureDefn->GetFieldDefn(i);
//        newLayer->CreateField(fieldDefn);
//    }

//    // копируем геометрию и значения атрибутов
//    layer->ResetReading();
//    OGRFeature* feature = nullptr;
//    while ((feature = layer->GetNextFeature()) != nullptr) {
//        OGRFeature* newFeature = new OGRFeature(featureDefn);
//        newFeature->SetFrom(feature);
//        newLayer->CreateFeature(newFeature);
//        delete newFeature;
//        delete feature;
//    }

//    delete targetCRS;
//    //layer=newLayer;
//    // Закройте слои и источник данных
//    dS->DeleteLayer(0);
//    dS->SetSpatialRef(targetCRS);

//    //layer->SyncToDisk();
//    newLayer->SyncToDisk();

    //GDALClose(dataSource);
}

void LIPVectorLayer::setZValue(int zValue)
{
    mZValue=zValue;
    update();
}

LIPPointLayer *LIPVectorLayer::toPointLayer()
{
    return dynamic_cast<LIPPointLayer*>(this);
}

LIPLineLayer *LIPVectorLayer::toLineLayer()
{
    return dynamic_cast<LIPLineLayer*>(this);
}


LIPPolygonLayer *LIPVectorLayer::toPolygonLayer()
{
    return dynamic_cast<LIPPolygonLayer*>(this);
}


QVector<LIPAttributeType> LIPVectorLayer::getAttributeTypes()
{
    OGRFeatureDefn* featureDefn = layer->GetLayerDefn();

    int attributeCount = featureDefn->GetFieldCount(); // Получение количества атрибутов
    for (int i = 0; i < attributeCount; i++)
    {
        OGRFieldDefn* fieldDefn = featureDefn->GetFieldDefn(i);
        OGRFieldType fieldType = fieldDefn->GetType(); // Получение типа данных
        //qDebug()<<fieldDefn->GetNameRef();
        switch (fieldType)
        {
        case OFTInteger:
            attributeTypes.append(LIPAttributeType::INT32);
            break;
        case OFTReal:
            attributeTypes.append(LIPAttributeType::Real);
            break;
        default:
            attributeTypes.append(LIPAttributeType::String);
            break;
        }
    }
    return attributeTypes;
}

QVector<QString> LIPVectorLayer::getAttributeNames()
{
    attributeNames.clear();
    OGRFeatureDefn* featureDefn = layer->GetLayerDefn();
    int attributeCount = featureDefn->GetFieldCount(); // Получение количества атрибутов
    for (int i = 0; i < attributeCount; i++)
    {
        OGRFieldDefn* fieldDefn = featureDefn->GetFieldDefn(i);
        attributeNames.append(QString::fromUtf8(fieldDefn->GetNameRef())); //получаем название атрибута
    }
    return attributeNames;

}

QStringList LIPVectorLayer::getAttributesNamesAsList()
{
    QStringList list;
    OGRFeatureDefn* featureDefn = layer->GetLayerDefn();
    int attributeCount = featureDefn->GetFieldCount(); // Получение количества атрибутов
    for (int i = 0; i < attributeCount; i++)
    {
        OGRFieldDefn* fieldDefn = featureDefn->GetFieldDefn(i);
        list.append(QString::fromUtf8(fieldDefn->GetNameRef())); //получаем название атрибута
    }
    return list;
}

QRectF LIPVectorLayer::getBoundingBox()
{
    OGREnvelope envelope;
    layer->GetExtent(&envelope, true);
    qreal minX = envelope.MinX;
    qreal minY = envelope.MinY;
    qreal maxX = envelope.MaxX;
    qreal maxY = envelope.MaxY;
    return QRectF(minX, minY, maxX - minX, maxY - minY);
}

QVector<LIPAttribute> LIPVectorLayer::stringValToAttrs(QVector<QString> names, QVector<QString> values, QVector<LIPAttributeType> types)
{
    QVector<LIPAttribute> attributes;
    bool typeFlag=false;
    for (int i=0; i<values.size(); i++)
    {
        if (values.at(i).trimmed().isEmpty())
        {
            LIPAttribute attr(names.at(i),types.at(i), QString());
            attributes.append(attr);

        }
        else
        {
            switch (types.at(i))
            {
            case LIPAttributeType::INT32:
                values.at(i).toInt(&typeFlag);
                if (typeFlag)
                {
                    LIPAttribute attr(names.at(i), types.at(i), values.at(i));
                    attributes.append(attr);
                }
                break;
            case LIPAttributeType::Real:
                values.at(i).toDouble(&typeFlag);
                if (typeFlag)
                {
                    LIPAttribute attr(names.at(i), types.at(i), values.at(i));
                    attributes.append(attr);
                }
                break;
            case LIPAttributeType::String:
                LIPAttribute attr(names.at(i), types.at(i), values.at(i));
                attributes.append(attr);
                typeFlag=true;
                break;
            }
            if (!typeFlag)
            {
                LIPAttribute attr(names.at(i), types.at(i), QString());
                attributes.append(attr);
            }
        }
    }
    return attributes;
}

LIPVectorStyle *LIPVectorLayer::getStyle()
{
    return mStyle;
}

void LIPVectorLayer::setMapFeatures()
{

}

OGRLayer *LIPVectorLayer::getOGRLayer()
{
    return layer;
}

void LIPVectorLayer::setSceneScaleFactor(double factor)
{

}

void LIPVectorLayer::selectFeature(int index)
{

}



