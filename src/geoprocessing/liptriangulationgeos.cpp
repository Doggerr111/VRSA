#include "liptriangulationgeos.h"

LIPTriangulationGeos::LIPTriangulationGeos()
{

}

QVector<QVector<QPointF>> LIPTriangulationGeos::getTriangulation(LIPVectorLayer *pointLayer)
{
    //проверки
    if (pointLayer==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Некорректный слой, триангуляция невозможна", 2000, messageStatus::Error);
        return QVector<QVector<QPointF>>();
    }
    LIPPointLayer* layer = dynamic_cast<LIPPointLayer*>(pointLayer);
    if (layer==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Слой не является точечным, триангуляция невозможна", 2000, messageStatus::Error);
        return QVector<QVector<QPointF>>();
    }
    //триангуляция
    QVector<QVector<QPointF>> trianglesVec; //координаты треугольников
    QVector<LIPPoint*> lipPoints= layer->returnCords();
    //geos::geom::GeometryFactory geometryFactory = geos::geom::GeometryFactory::getDefaultInstance();
    geos::triangulate::DelaunayTriangulationBuilder geosTr;
    //конвертируем в geos формат
    geosTr.setSites(LIPVectorConvertor::LIPPointsToGeosCoordinateSequence(lipPoints));
    //geos::geom::GeometryFactory factory();

    geos::geom::GeometryFactory::Ptr gfact = geos::geom::GeometryFactory::create();
    std::unique_ptr<geos::geom::GeometryCollection> triangles= geosTr.getTriangles(*gfact);
    std::vector<Coordinate> triCoords;
    triangles->getCoordinates()->toVector(triCoords);
    int count=0;

    QVector<QPointF> tempVec;
    for (const auto coordinate: triCoords)
    {
        if (count==3) //пропускаем каждую 4 координату, так как она повторяется с одной из координат треугольника
        {
            trianglesVec.append(tempVec);
            count=0;
            tempVec.clear();
            continue;
        }
        tempVec.append(QPointF(coordinate.x,coordinate.y));
        count++;
    }
    return trianglesVec;
//    for(int i=0; i<triangles->getLength()-1; i++)
//    {
//        qDebug()<<"check getGeometryN";
//        auto tr = triangles->getGeometryN(i)->getCoordinates();
//        triangles->getCoordinates();

//        QVector<QPointF> tempVec;
//        for( uint k=0; k<tr->size()-1; k++)
//        {
//            qDebug()<<"check Point";
//            QPointF point(tr->getAt(k).x, tr->getAt(k).y);
//            qDebug()<<point;
//            tempVec.append(point);

//        }
//        trianglesVec.append(tempVec);
//    }
//    return trianglesVec;
    //std::unique_ptr<geos::geom::CoordinateSequence> coordinateSequence = triangles->getCoordinates();

    //LIPLayerCreator *lCr=new LIPLayerCreator(LIPGeometryType::LIPPolygon, fileName, "outputLayer",
      //                                           dynamic_cast<LIPCoordinateSystem*>(layer->getOGRLayer()->GetSpatialRef()));
    //LIPVectorLayer* outputLayer = lCr->returnLayer();


//    for (unsigned int i = 0; i < coordinateSequence->getSize(); ++i) {
//        const geos::geom::Coordinate& coordinate = coordinateSequence->getAt(i);
//        //outputLayer->addFeature()
//        //qDebug()<<QPointF(coordinate.x, coordinate.y);
//    }


    //qDebug()<<coordinateSequence->getSize();

}
