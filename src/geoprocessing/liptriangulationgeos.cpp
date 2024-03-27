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

}

QVector<QVector<QPointF> > LIPTriangulationGeos::getVoronoiDiagram(LIPVectorLayer *pointLayer)
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
    QVector<QVector<QPointF>> polygonsVec; //координаты полигонов
    QVector<LIPPoint*> lipPoints= layer->returnCords();
    //geos::geom::GeometryFactory geometryFactory = geos::geom::GeometryFactory::getDefaultInstance();
    geos::triangulate::DelaunayTriangulationBuilder geosTr;
    geos::triangulate::VoronoiDiagramBuilder geosVD;
    geosVD.setSites(LIPVectorConvertor::LIPPointsToGeosCoordinateSequence(lipPoints));

    geos::geom::GeometryFactory::Ptr gfact = geos::geom::GeometryFactory::create();
    std::unique_ptr<geos::geom::GeometryCollection> polygons= geosVD.getDiagram(*gfact);

    for(std::size_t i=0; i<polygons->getLength(); i++)
    {

        auto poly = polygons->getGeometryN(i);
        if (poly==nullptr)
            break;
        auto polyCords = poly->getCoordinates();
        QVector<QPointF> tempVec;
        for(std::size_t n=0; n<polyCords->getSize(); n++)
        {
            auto point = polyCords->getAt(n);
            tempVec.append(QPointF(point.x, point.y));
        }
        polygonsVec.append(tempVec);

    }

    return polygonsVec;
    //return QVector<QVector<QPointF>>();
}
