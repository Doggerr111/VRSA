#include "liptile.h"
#include <QDebug>
LIPTile::LIPTile(int zoom, int x, int y):
    mX{x},
    mY{y},
    mZoomLevel{zoom},
    mResolution{256},
    mItem{nullptr}
{
    originShift = 2 * M_PI * 6378137 / 2.0;
    initialRes = 2 * M_PI * 6378137 / mResolution;
}

LIPTile::~LIPTile()
{
    delete mItem;
}

void LIPTile::setResolution(int sizePixels)
{
    mResolution = sizePixels;
}

QRectF LIPTile::calculateBoundsLatLon()
{
    double topLat = tileYToLatitude(mY, mZoomLevel);
    double leftLon = tileXToLongitude(mX, mZoomLevel);
    double bottomLat = tileYToLatitude(mY+1, mZoomLevel);
    double rightLon = tileXToLongitude(mX+1, mZoomLevel);
    qDebug()<<QRectF(topLat,leftLon,bottomLat,rightLon);

    OGRSpatialReference sourceSRS, targetSRS;
    sourceSRS.importFromEPSG(4326);
    targetSRS.importFromEPSG(3857);

    // Создание объекта преобразования координат из EPSG 4326 в EPSG 3857
    OGRCoordinateTransformation *poCT = OGRCreateCoordinateTransformation(&sourceSRS, &targetSRS);
//    poCT->Transform(1,  &bottomLat, &leftLon);
//    poCT->Transform(1, &topLat, &rightLon);
    //bottomLat=topLat;
    //rightLon=leftLon;
    poCT->Transform(1, &topLat, &leftLon);
    //std::swap(leftLon,topLat);
    //rightLon = 91.999997;
    //bottomLat = 1.0;
    //rightLon=90;
    //bottomLat=180;
    poCT->Transform(1, &bottomLat, &rightLon);
    //std::swap(rightLon,bottomLat);

//    OGRSpatialReference sourceSRS, targetSRS;
//        sourceSRS.importFromEPSG(4326); // WGS 84
//        targetSRS.importFromEPSG(3857); // EPSG 3857

//        // Создание объекта для преобразования координат
//        OGRCoordinateTransformation *transform = OGRCreateCoordinateTransformation(&sourceSRS, &targetSRS);

//        if (transform)
//        {
//            // Исходные координаты
//            double x = 179.0;
//            double y = 89.051128;

//            // Перепроекция координат
//            transform->Transform(1, &y, &x);
//            std::swap(x,y);
//            qDebug()<<QString::number(x, 'f', 3)<<QString::number(y, 'f', 3);



//        }



//    qDebug()<<QRectF(topLat,leftLon,bottomLat,rightLon);

//    qDebug()<<QString::number(topLat,'f',3)<<QString::number(leftLon,'f',3)
//           <<QString::number(bottomLat,'f',3)<<QString::number(rightLon,'f',3);

    //return QRectF(topLat, rightLon, bottomLat, leftLon);
    //qDebug()<< "test"<<QRectF(leftLon, topLat, rightLon, bottomLat);

    qDebug()<<"test"<<QString::number(topLat,'f',3)<<QString::number(leftLon,'f',3)
           <<QString::number(bottomLat,'f',3)<<QString::number(rightLon,'f',3);
    return QRectF(topLat, leftLon, bottomLat, rightLon);
    //return QRectF(leftLon,bottomLat, rightLon, topLat);
}

double LIPTile::tileXToLongitude(int x, int zoom)
{
    //return (x / pow(2.0, zoom) * 360.0) - 180.0;
    return x / ( double )( 1 << zoom ) * 360.0 - 180;
}

double LIPTile::tileYToLatitude(int y, int zoom)
{
    double n = M_PI - (2.0 * M_PI * y) / (double) (1<<zoom);
    return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

int LIPTile::resolution()
{
    return initialRes/std::pow(2,mZoomLevel);
}

void LIPTile::setGraphicsItem(QPixmap pixmap)
{
    if (mItem == nullptr)
        mItem = new QGraphicsPixmapItem;
    mItem->setPixmap(pixmap);
    QRectF bRect = calculateBoundsLatLon();
    //calculateBoundsLatLon();
    double topLat=bRect.x();
    double leftLon=bRect.y();
    double bottomLat=bRect.width();

    double rightLon=bRect.height();
    //qDebug()<<rightLon;
    //bRect.getCoords(&topLat, &leftLon, &bottomLat, &rightLon);
    //qDebug()<< "scaleTest"<<topLat-bottomLat;
    //qDebug()<< "scaleTest"<<rightLon-leftLon;


//    double width = fabs(rightLon- leftLon);
//    double height = fabs(bottomLat - topLat);

//    double currentWidth = pixmap.width();  // предполагается, что pixmapItem является QGraphicsPixmapItem
//    double currentHeight = pixmap.height();

//    double scaleX = width / currentWidth;
//    double scaleY = height / currentHeight;

    QRectF pixmapRect = mItem->boundingRect();

    qDebug()<<"test"<<QString::number(topLat,'f',3)<<QString::number(leftLon,'f',3)
           <<QString::number(bottomLat,'f',3)<<QString::number(rightLon,'f',3);
    double scaleX = (bottomLat-topLat) / pixmapRect.width();
    double scaleY = (bRect.height()-bRect.y()) / pixmapRect.height();
    qDebug()<<scaleX;
    QTransform geoTr;
    //geoTr.translate(topLat, leftLon);
    mItem->setPos(topLat, leftLon);
    geoTr.scale(scaleX, -scaleX);
    mItem->setTransform(geoTr);

    //mItem->setScale(qMin(scaleX, scaleY)); // устанавливаем минимальное значение для сохранения соотношения сторон

    // Центрирование pixmapItem в пределах targetRect
    double xOffset = (bRect.width() - pixmapRect.width() * mItem->scale()) / 2.0;
    double yOffset = (bRect.height() - pixmapRect.height() * mItem->scale()) / 2.0;
    //mItem->setPos(bRect.topLeft() + QPointF(xOffset, yOffset));
    LIPWidgetManager::getInstance().getScene()->addItem(mItem);
    //LIPWidgetManager::getInstance().getView()->centerOn(topLat, leftLon);



}

QGraphicsPixmapItem *LIPTile::getItem()
{
    return mItem;
}

QRectF LIPTile::tileLatLonBounds()
{
    QRectF bounds = tileBounds();
    QPointF min = metersToLatLon(bounds.x(), bounds.y());
    QPointF max = metersToLatLon(bounds.width(), bounds.height());

    return ( QRectF(min.x(),min.y(),max.x(), max.y()));
}

QPointF LIPTile::metersToLatLon(double mx, double my)
{
    //static const double originShift = 2 * M_PI * 6378137 / 2.0;
    double lon = (mx / originShift) * 180.0;
    double lat = (my / originShift) * 180.0;

    lat = 180 / M_PI * (2 * std::atan( std::exp( lat * M_PI / 180.0)) - M_PI / 2.0);
    return QPointF(lon,lat);
}

QPointF LIPTile::pixelsToMeters(double px, double py)
{
    int res = resolution( );
    return QPointF(px * res - originShift, py * res - originShift);

}

QRectF LIPTile::tileBounds()
{
    QPointF min = pixelsToMeters( mX*mResolution, (-mY+(std::pow(2,mZoomLevel)+1))*mResolution );
    QPointF max = pixelsToMeters( (mX+1)*mResolution, ((-mY-1)+ (std::pow(2,mZoomLevel)+1))*mResolution);



//    double n = pow(2.0, mZoomLevel);
//    double lon_deg = mX / n * 360.0 - 180.0;
//    double lat_rad = atan(sinh(M_PI * (1 - 2 * mY / n)));
//    double lat = lat_rad * 180.0 / M_PI;
//    double lon = lon_deg;

//    lon_deg = mX+1 / n * 360.0 - 180.0;
//    lat_rad = atan(sinh(M_PI * (1 - 2 * mY-1 / n)));
//    double latMax = lat_rad * 180.0 / M_PI;
//    double lonMax = lon_deg;


//    const char *pszDstWKT = "EPSG:3857";
//    OGRSpatialReference oSRS;
//    //oSRS.SetWellKnownGeogCS(pszDstWKT);
//    oSRS.importFromEPSG(3857);

//    const char *DstWKT = "EPSG:4326";
//    OGRSpatialReference iSRS;
//    iSRS.SetWellKnownGeogCS("WGS84");

//    // Создание объекта преобразования координат из EPSG 4326 в EPSG 3857
//    OGRCoordinateTransformation *poCT = OGRCreateCoordinateTransformation(&iSRS, &oSRS);
//    poCT->Transform(1, &lat, &lon);
//    poCT->Transform(1, &latMax, &lonMax);

//    mY=2362;
//    double n = 1 << mZoomLevel;
//    double lon_deg = mX / n * 360.0 - 180.0;
//    double lat_rad = atan(sinh(M_PI * (1 - 2 * mY / n)));

//    double lat = lon_deg * 20037508.34 / 180.0;
//    double lon = log(tan((90 + lat_rad) * M_PI / 360.0)) / (M_PI / 180.0);
//    lon = lon * 20037508.34 / 180.0;
//    qDebug()<<lat<<lon;


    qDebug()<< "TILE COORDINATE IS :" << QString::number(min.x(), 'f', 3) << " " <<QString::number(min.y(), 'f', 3);

    return ( QRectF(min.x(), min.y(), max.x(), max.y()) );
    //return ( QRectF(lon, lat, lonMax, latMax) );
}

QPointF LIPTile::metersToPixels()
{
//    double res = resolution();
//    mx = px * res - self.originShift
//    my = py * res - self.originShift
//    return mx, my
}




//def __init__(self, tileSize=256):
//     "Initialize the TMS Global Mercator pyramid"
//     self.tileSize = tileSize
//     self.initialResolution = 2 * math.pi * 6378137 / self.tileSize
//     # 156543.03392804062 for tileSize 256 pixels
//     self.originShift = 2 * math.pi * 6378137 / 2.0
//     # 20037508.342789244

// def MetersToLatLon(self, mx, my ):
//     "Converts XY point from Spherical Mercator EPSG:900913 to lat/lon in WGS84 Datum"

//     lon = (mx / self.originShift) * 180.0
//     lat = (my / self.originShift) * 180.0

//     lat = 180 / math.pi * (2 * math.atan( math.exp( lat * math.pi / 180.0)) - math.pi / 2.0)
//     return lat, lon

// def PixelsToMeters(self, px, py, zoom):
//     "Converts pixel coordinates in given zoom level of pyramid to EPSG:900913"

//     res = self.Resolution( zoom )
//     mx = px * res - self.originShift
//     my = py * res - self.originShift
//     return mx, my

// def TileBounds(self, tx, ty, zoom):
//     "Returns bounds of the given tile in EPSG:900913 coordinates"

//     minx, miny = self.PixelsToMeters( tx*self.tileSize, ty*self.tileSize, zoom )
//     maxx, maxy = self.PixelsToMeters( (tx+1)*self.tileSize, (ty+1)*self.tileSize, zoom )
//     return ( minx, miny, maxx, maxy )

// def TileLatLonBounds(self, tx, ty, zoom ):
//     "Returns bounds of the given tile in latutude/longitude using WGS84 datum"

//     bounds = self.TileBounds( tx, ty, zoom)
//     minLat, minLon = self.MetersToLatLon(bounds[0], bounds[1])
//     maxLat, maxLon = self.MetersToLatLon(bounds[2], bounds[3])

//     return ( minLat, minLon, maxLat, maxLon )

// def Resolution(self, zoom ):
//     "Resolution (meters/pixel) for given zoom level (measured at Equator)"

//     # return (2 * math.pi * 6378137) / (self.tileSize * 2**zoom)
//     return self.initialResolution / (2**zoom)
