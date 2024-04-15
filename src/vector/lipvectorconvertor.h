#ifndef LIPVECTORCONVERTOR_H
#define LIPVECTORCONVERTOR_H

#include <geos/geom/Coordinate.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Envelope.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos.h>
#include "gdal.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include "QPointF"
#include "geos/geom/Point.h"
#include "geos/operation/intersection/RectangleIntersection.h"
#include "geos/operation/intersection/Rectangle.h"
#include "lippoint.h"
#include <geos/geom/Polygon.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Geometry.h>
#include <geos/operation/overlay/OverlayOp.h>
#include <liptypes.h>
#include <geos/io/WKTReader.h>

/** Данный класс используется для конвертации объектов GDAL в GEOS и наборот, а также для конвертации в Qt и LIP*/
class LIPVectorConvertor
{
public:
    LIPVectorConvertor();
    static std::unique_ptr<Point> QPointFtoGeosPoint(QPointF qtPoint);
    static geos::geom::CoordinateSequence LIPPointsToGeosCoordinateSequence(QVector<LIPPoint*> lipPoints);
    static QVector<LIPPoint*> QPointsFtoLIPPoints(QVector<QPointF> qtPoints);

    static geos::geom::GeometryFactory buildPolygonFromCoordinateSeq(geos::geom::CoordinateSequence);
    /** Получение geos::geom::Geometry для линий/полигонов */
    static std::unique_ptr<geos::geom::Geometry> getGeosGeometryFromCoordinates(QVector<QVector<LIPPoint*>> cords, LIPGeometryType geomType);
    /** Получение geos::geom::Geometry для точек */
    static geos::geom::Geometry getGeosGeometryFromCoordinates(QVector<LIPPoint*> cords);
    //point to geos
    static std::unique_ptr<geos::geom::MultiPolygon> vectorPointstoGeosPolygon(QVector<QVector<LIPPoint*>> lipPoints);
    static std::unique_ptr<geos::geom::MultiPoint> vectorPointstoGeosPoints(QVector<LIPPoint*> lipPoints);
    static std::unique_ptr<geos::geom::MultiLineString> vectorPointstoGeosLine(QVector<QVector<LIPPoint*>> lipPoints);
//    static geos::geom::Point *QPointFtoGeosPoint(QPointF* qtPoint);
//    static QPointF geosPointtoQPointF(geos::geom::Point geosPoint);
//    static QPointF* geosPointtoQPointF(geos::geom::Point* geosPoint);
//    //for set of points
//    static QVector<geos::geom::Point> QPointsFtoGeosPoints(QVector<QPointF> qtPoints);
};

#endif // LIPVECTORCONVERTOR_H
