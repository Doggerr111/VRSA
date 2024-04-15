#include "lipvectorconvertor.h"
#include <QDebug>
LIPVectorConvertor::LIPVectorConvertor()
{

}

std::unique_ptr<geos::geom::Point> LIPVectorConvertor::QPointFtoGeosPoint(QPointF qtPoint)
{
    using namespace geos::geom;
    geos::geom::Coordinate coordinate;
    coordinate.x=qtPoint.x();
    coordinate.y=qtPoint.y();
    //geos::geom::Point geoPoint(coordinate,);

    GeometryFactory::Ptr factory = GeometryFactory::create();
    return factory->createPoint(coordinate);
    //geos::geom::Point* point = new geos::geom::Point(coordinate);

    ///

}

geos::geom::CoordinateSequence LIPVectorConvertor::LIPPointsToGeosCoordinateSequence(QVector<LIPPoint *> lipPoints)
{
//    std::initializer_list<geos::geom::Coordinate> coords;
//    for(int i=0; i<lipPoints.size(); i++)
//    {
//        geos::geom::Coordinate tempCord(lipPoints.at(i)->x(), lipPoints.at(i)->y());
//        //coords.insert

//    }

    //geos::geom::CoordinateSequence* factory();

    std::vector<geos::geom::Coordinate> coordinates;
    for (const QPointF* point : lipPoints)
    {
        geos::geom::Coordinate coordinate(point->x(), point->y());
        coordinates.push_back(coordinate);
    }
    geos::geom::CoordinateSequence sequence(static_cast<size_t>(lipPoints.size()));
    sequence.setPoints(coordinates);
    return sequence;
    //geos::geom::CoordinateSequence f;
    //pts->setPoints(coordinates);
    //geos::geom::CoordinateSequence();
}

QVector<LIPPoint *> LIPVectorConvertor::QPointsFtoLIPPoints(QVector<QPointF> qtPoints)
{
    QVector<LIPPoint*> LIPVector;
    foreach (QPointF qP, qtPoints)
    {
        LIPPoint* tempP = new LIPPoint;
        tempP->setX(qP.x());
        tempP->setY(qP.y());
        LIPVector.append(tempP);
    }
    return LIPVector;
}

GeometryFactory LIPVectorConvertor::buildPolygonFromCoordinateSeq(CoordinateSequence seq)
{
    using namespace geos::geom;
    GeometryFactory::Ptr factory = GeometryFactory::create();
    auto envelope = geos::geom::Envelope(1,1,1,1);
    auto rectangle = factory->toGeometry(&envelope);
    //auto lR = factory->createLinearRing(seq);

    //factory->createPolygon(f);
    auto poly = factory->createPolygon(seq.getSize());


    //return factory;
    // geos::operation::intersection::RectangleIntersection inter(factory->toGeometry(), f);
}

std::unique_ptr<geos::geom::Geometry> LIPVectorConvertor::getGeosGeometryFromCoordinates(QVector<QVector<LIPPoint *> > cords, LIPGeometryType geomType)
{

    auto factory = geos::geom::GeometryFactory::create();
    geos::io::WKTReader reader(factory.get());
    QString wktFormat;
    switch (geomType)
    {

        case LIPGeometryType::LIPLineString: //если работаем с линейным слоем (его координатами)
        {
            wktFormat.append("LINESTRING(");
            for (auto vec:cords)
            {
                wktFormat.append("(");
                for (auto point:vec)
                {
                    wktFormat.append(QString::number(point->x())+" " + QString::number(point->y())+ ",");
                }
                wktFormat.chop(1);
                wktFormat.append(")");
            }
            wktFormat.append(")");
            break;
        }
        case LIPGeometryType::LIPPolygon: //если работаем с линейным слоем (его координатами)
        {
            wktFormat.append("MULTIPOLYGON(");
            for (auto vec:cords)
            {
                wktFormat.append("(");
                for (auto point:vec)
                {
                    wktFormat.append(QString::number(point->x())+" " + QString::number(point->y())+ ",");
                }
                wktFormat.chop(1);
                wktFormat.append(")");
            }
            wktFormat.append(")");
            break;
        }
        default:
            break;
    }
    //auto interResult = reader.read(wktFormat.toStdString())->intersection(reader.read(wktFormat.toStdString()).get());
    return reader.read(wktFormat.toStdString());


}

std::unique_ptr<MultiPolygon> LIPVectorConvertor::vectorPointstoGeosPolygon(QVector<QVector<LIPPoint *> > lipPoints)
{
    auto geomFac = geos::geom::GeometryFactory::create();
    std::vector<std::unique_ptr<geos::geom::Polygon>> geosPolygons;
    for (auto vec: lipPoints)
    {
        if (vec.size()>=3) //проверка, что в слое нет некорректных полигонов
        {
            geos::geom::CoordinateSequence points=LIPPointsToGeosCoordinateSequence(vec);
            points.add(vec[0]->x(), vec[0]->y()); //ЗАКРЫВАЕМ ПОЛИГОН!!
            qDebug()<<points.size();
            auto poly=geomFac->createPolygon(std::move(points));
            geosPolygons.push_back(std::move(poly));
        }
    }
    return geomFac->createMultiPolygon(std::move(geosPolygons));
}

std::unique_ptr<MultiPoint> LIPVectorConvertor::vectorPointstoGeosPoints(QVector<LIPPoint *> lipPoints)
{
    auto geomFac = geos::geom::GeometryFactory::create();
    std::vector<std::unique_ptr<geos::geom::Point>> geosPoints;
    for (auto point: lipPoints)
    {
        geos::geom::Coordinate coordinate(point->x(), point->y());
        auto geosPoint=geomFac->createPoint(coordinate);
        geosPoints.push_back(std::move(geosPoint));

    }
    return geomFac->createMultiPoint(std::move(geosPoints));
}

std::unique_ptr<MultiLineString> LIPVectorConvertor::vectorPointstoGeosLine(QVector<QVector<LIPPoint *> > lipPoints)
{
    auto geomFac = geos::geom::GeometryFactory::create();
    std::vector<std::unique_ptr<geos::geom::LineString>> geosLines;

    for (auto vec: lipPoints)
    {
        if (vec.size()>=2) //проверка, что в слое нет некорректных линий
        {

            geos::geom::CoordinateSequence points=LIPPointsToGeosCoordinateSequence(vec);

            auto line=geomFac->createLineString(std::move(points));
            geosLines.push_back(std::move(line));
        }
    }

    return geomFac->createMultiLineString(std::move(geosLines));
}


//geos::geom::Point LIPVectorConvertor::QPointFtoGeosPoint(QPointF qtPoint)
//{
    //geos::geom::Coordinate coord;
    //geos::geom::GeometryFactory* factory = geos::geom::GeometryFactory::create().get();
    //geos::geom::Point* point =  factory->createPoint(coord);
    //qDebug()<<(point->getX());

//}

//geos::geom::Point *LIPVectorConvertor::QPointFtoGeosPoint(QPointF *qtPoint)
//{

//}

//QVector<geos::geom::Point> LIPVectorConvertor::QPointsFtoGeosPoints(QVector<QPointF> qtPoints)
//{

//}
