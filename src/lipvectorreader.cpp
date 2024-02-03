#include "lipvectorreader.h"


LIPVectorReader::LIPVectorReader(const LIPTypes::LIPDrivers driver, const char* new_filename)
    :driver{driver},
     fileName{new_filename}

{
    OGRRegisterAll();
}

bool LIPVectorReader::Read()
{
    //используем GDAL для чтения файлов
    switch (driver) {
       case LIPTypes::LIPDrivers::ESRIShapefile:
    {
            char** ppszOptions = NULL;
            ppszOptions = CSLSetNameValue(ppszOptions, "ENCODING", "UTF-8");
            CPLSetConfigOption("SHAPE_ENCODING","");
            GDALDataset *shpDS = (GDALDataset *)GDALOpenEx(fileName, GDAL_OF_VECTOR, NULL, NULL, NULL);
            //shpDS = (GDALDataset *)GDALOpen(fileName, GA_Update);
            if (shpDS == NULL)
            {
                qDebug()<<"Error:cant read this shp file: " + QString(fileName);
                return false;
            }
            int c=shpDS->GetLayers().size();
            qDebug()<<QString::number(c);
            OGRLayer *shpLayer = shpDS->GetLayer(0);
            shpLayer->GetName();
            if (shpLayer==nullptr)
                qDebug()<<"nullptr layer";
            OGRFeature *shpFeature;
            shpLayer->ResetReading();
            int counter=0;
            //qDebug()<<shpLayer->GetSpatialRef()->GetEPSGGeogCS();
            while ((shpFeature = shpLayer->GetNextFeature()) != NULL)
            {
                qDebug()<<QString::number(counter);
                counter++;
                OGRFeatureDefn *poFDefn = shpLayer->GetLayerDefn();
                int iField;
                for (iField = 0; iField < poFDefn->GetFieldCount(); iField++)
                {
                    OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn(iField);
                    if (poFieldDefn->GetType() == OFTInteger)
                        qDebug()<<"%d, " +  QString::number(shpFeature->GetFieldAsInteger(iField));
                    else if (poFieldDefn->GetType() == OFTInteger64)
                        qDebug()<<"%lld, "+QString::number( shpFeature->GetFieldAsInteger64(iField));
                    else if (poFieldDefn->GetType() == OFTReal)
                        qDebug()<<"%.3f, "+QString::number(shpFeature->GetFieldAsDouble(iField));
                    else if (poFieldDefn->GetType() == OFTString)
                        qDebug()<<"%s, " + QString(shpFeature->GetFieldAsString(iField));
                    else
                        qDebug()<<"%s, " + QString(shpFeature->GetFieldAsString(iField));
                }
                //printf("\n");
                OGRGeometry *poGeometry = shpFeature->GetGeometryRef();
                int count = shpFeature->GetGeomFieldCount();

                for (int i=0;i<shpFeature->GetGeomFieldCount();i++)
                {
                if (poGeometry != NULL)
                {
                    OGRwkbGeometryType type=poGeometry->getGeometryType();

                    switch (poGeometry->getGeometryType())
                    {
                    case wkbPoint:
                    {
                        OGRPoint *point=(OGRPoint *)poGeometry;
                        //qDebug()<<QString::number(point->getX());
                        //qDebug()<<"так точнооо";

                    }

//                    case wkbMultiPoint:
//                    {

//                        OGRMultiPoint *multiPoint = (OGRMultiPoint *)poGeometry;
//                        OGRPoint *point=(OGRPoint *)poGeometry;
//                        qDebug()<<QString::number(point->getX());
//                        for (int n = 0; n < multiPoint->getNumGeometries(); n++)
//                        {

//                            OGRPoint *point = (OGRPoint *)multiPoint->getGeometryRef(n);
//                            qDebug()<<"wkbMultiPoint" + QString::number(point->getX()), point->getY(), point->getZ();

//                        }
//                    }
                    case wkbLineString:
                    {
                        OGRLineString *line = (OGRLineString *)poGeometry;
                        for (int i = 0; i < line->getNumPoints(); i++)
                        {
                            qDebug()<<"wkbLineString %d: x=%g y=%g z=%g\n", i, line->getX(i), line->getY(i), line->getZ(i);
                        }
                        break;
                    }
                    case wkbPolygon:
                    {
                        OGRPolygon *poly = (OGRPolygon *)poGeometry;
                        OGRLinearRing *ring = (OGRLinearRing *)poly->getExteriorRingCurve();
                        for (int i = 0; i < ring->getNumPoints(); i++)
                        {
                            qDebug()<<"wkbPolygon %d: x=%g y=%g z=%g\n" +QString::number(i) + QString::number( ring->getX(i)), ring->getY(i), ring->getZ(i);
                        }
                        break;
                    }
                    case wkbMultiLineString:
                    {
                        OGRMultiLineString *multiLine = (OGRMultiLineString *)poGeometry;
                        for (int n = 0; n < multiLine->getNumGeometries(); n++)
                        {
                            OGRLineString *line = (OGRLineString *)multiLine->getGeometryRef(n);
                            for (int i = 0; i < line->getNumPoints(); i++)
                            {
                                qDebug()<<"wkbMultiLineString %d-%d: x=%g y=%g z=%g\n", n, i, line->getX(i), line->getY(i), line->getZ(i);
                            }
                        }

                        break;
                    }
                    default:
                        qDebug()<<"no point geometry\n";
                        break;
                    }
                }
                }

                OGRFeature::DestroyFeature(shpFeature);
            }


            GDALClose(shpDS);
    }

    }
    return true;
}

bool LIPVectorReader::ReadGeometry()
{
    GDALDataset *shpDS = (GDALDataset *)GDALOpenEx(fileName,GDAL_OF_VECTOR, NULL, NULL, NULL);
    //shpDS = (GDALDataset *)GDALOpen(fileName, GA_Update);
    if (shpDS == NULL)
    {
        qDebug()<<"Error:cant read this shp file: " + QString(fileName);
        return false;
    }

    OGRLayer *shpLayer = shpDS->GetLayer(0);


    if (shpLayer==nullptr)
        qDebug()<<"nullptr layer";
    OGRFeature *shpFeature;
    shpLayer->ResetReading();
    int counter=0;
    //qDebug()<<shpLayer->GetSpatialRef()->GetEPSGGeogCS();
    while ((shpFeature = shpLayer->GetNextFeature()) != NULL)
    {
        OGRGeometry* geom=shpFeature->GetGeometryRef();

        switch(geom->getGeometryType())
        {
            case (wkbPoint):
        {
            OGRPoint *point=(OGRPoint*)geom;
            //qDebug()<<"x_coordinate: "+ QString::number(point->getX()) + "; y_cooordinate: "+ QString::number(point->getY());
            LIPPoint *lipp = new LIPPoint();
            lipp->setX(point->getX());
            lipp->setY(point->getY());
            qDebug()<<"Coordinates: " + QString::number(point->getX()) + " " + QString::number(point->getY());

            //обьекты карты

            //layer->addPoint(lipp);
            //sleep(10);


        }
        }
    }

    return true;
}

QRectF LIPVectorReader::ReadBoundingBox()
{
    GDALDataset *shpDS = (GDALDataset *)GDALOpenEx(fileName, GDAL_OF_VECTOR, NULL, NULL, NULL);
    if (shpDS == NULL)
    {
        qDebug()<<"Error:cant read this shp file: " + QString(fileName);

    }
    else{
    OGRLayer *shpLayer = shpDS->GetLayer(0);
    OGREnvelope *ext = new OGREnvelope;
    int f=shpLayer->GetExtent(ext);
    QRectF rect(ext->MinX,ext->MinY,ext->MaxX,ext->MaxY);
    qDebug()<<rect;
    return rect;
//    if (shpLayer==nullptr)
//        qDebug()<<"nullptr layer";
//    OGRFeature *shpFeature;
//    while ((shpFeature = shpLayer->GetNextFeature()) != NULL)
//    {
//    //OGRFeature *shpFeature=shpLayer->GetNextFeature();
//    OGRGeometry* geom=shpFeature->GetGeometryRef();

//    OGRGeometry*rect=geom->Boundary();
//    OGRPolygon *poly=(OGRPolygon*)rect;
//    OGRLinearRing *ring = (OGRLinearRing *)poly->getExteriorRingCurve();
//    for (int i = 0; i < ring->getNumPoints(); i++)
//    {
//        qDebug()<<"wkbPolygon %d: x=%g y=%g z=%g\n" +QString::number(i) + QString::number( ring->getX(i)), ring->getY(i), ring->getZ(i);
//    }
//    }
//    }
//    return QRectF(1,1,1,1);
}

}



QPair<OGRLayer*, GDALDataset*> LIPVectorReader::readOGRLayer(QString filename)
{
    OGRRegisterAll();
    QByteArray ba = filename.toLocal8Bit();
    const char *nameChar = ba.data();
    char** ppszOptions = NULL;
    ppszOptions = CSLSetNameValue(ppszOptions, "ENCODING", "UTF-8");
    CPLSetConfigOption("SHAPE_ENCODING","");
    GDALDataset *shpDS = (GDALDataset *)GDALOpenEx(nameChar, GDAL_OF_VECTOR |  GDAL_OF_UPDATE, NULL, NULL, NULL);


    //shpDS = (GDALDataset *)GDALOpen(nameChar, GA_Update);
    if (shpDS == NULL)
    {
       // qDebug()<<"Error:cant read this shp file: " + QString(filename);


    }
    int c=shpDS->GetLayers().size();
    qDebug()<<QString::number(c);
    OGRLayer *l = shpDS->GetLayer(0);
    QPair<OGRLayer*, GDALDataset*> f;
    f.first=l;
    f.second=shpDS;
    return f;
}

QVector<OGRLayer *> LIPVectorReader::readLayersFromDataset(GDALDataset *ds)
{
    QVector<OGRLayer*> layerVector;
    if (ds==nullptr)
        return layerVector;
        //return QVector<OGRLayer*>();

    for(int i=0; i<static_cast<int>(ds->GetLayers().size()); i++)
    {
        layerVector.append(ds->GetLayer(i));
    }
    return layerVector;

}

LIPGeometryType LIPVectorReader::readGeometryType(OGRLayer *layer)
{
    switch (layer->GetGeomType())
    {
    case wkbPoint:
    {
        return LIPGeometryType::LIPPoint;
        break;
    }

    case wkbLineString:
    {
        return LIPGeometryType::LIPLineString;
        break;
    }

    case wkbPolygon:
    {
        return LIPGeometryType::LIPPolygon;
        break;
    }
    default:
        break;
    }

}

std::map<int, QVector<LIPAttribute>>LIPVectorReader::readAttributes(OGRLayer *shpLayer)
{
    std::map<int, QVector<LIPAttribute>> f;

    OGRFeature *shpFeature;
    shpLayer->ResetReading();
    int counter=0;
    //qDebug()<<shpLayer->GetSpatialRef()->GetEPSGGeogCS();
    while ((shpFeature = shpLayer->GetNextFeature()) != NULL)
    {
        qDebug()<<QString::number(counter);

        OGRFeatureDefn *poFDefn = shpLayer->GetLayerDefn();
        int iField;
        QVector<LIPAttribute> attrs;
        for (iField = 0; iField < poFDefn->GetFieldCount(); iField++)
        {

            OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn(iField);
            if (poFieldDefn->GetType() == OFTInteger)
                attrs.append(LIPAttribute(poFieldDefn->GetNameRef(),LIPAttributeType::INT32, shpFeature->GetFieldAsInteger(iField)));
            else if (poFieldDefn->GetType() == OFTInteger64)
                attrs.append(LIPAttribute(poFieldDefn->GetNameRef(),LIPAttributeType::INT32, shpFeature->GetFieldAsInteger64(iField)));
            else if (poFieldDefn->GetType() == OFTReal)
                attrs.append(LIPAttribute(poFieldDefn->GetNameRef(),LIPAttributeType::Real, shpFeature->GetFieldAsDouble(iField)));
            else if (poFieldDefn->GetType() == OFTString)
                attrs.append(LIPAttribute(poFieldDefn->GetNameRef(),LIPAttributeType::String, shpFeature->GetFieldAsString(iField)));
            else
                attrs.append(LIPAttribute(poFieldDefn->GetNameRef(),LIPAttributeType::String, shpFeature->GetFieldAsString(iField)));
        }
        f.insert(std::make_pair(counter, attrs));
        attrs.clear();
        counter++;
    }
    return f;

}
