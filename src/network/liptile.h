#ifndef LIPTILE_H
#define LIPTILE_H
#include <QGraphicsPixmapItem>
#include <math.h>
#include <lipwidgetmanager.h>
#include <ogr_spatialref.h>

class LIPTile
{
public:
    LIPTile(int zoomLevel, int x, int y);
    ~LIPTile();
    void setResolution(int sizePixels);
    QRectF calculateBoundsLatLon();
    double tileXToLongitude(int x, int zoom);
    double tileYToLatitude(int y, int zoom);
    int resolution();
    void setGraphicsItem(QPixmap pixmap);
    QGraphicsPixmapItem* getItem();
    QRectF tileLatLonBounds();
    QPointF metersToLatLon(double mx, double my);
    QPointF pixelsToMeters(double px, double py);
    QRectF tileBounds();
    QPointF metersToPixels();
private:
    int mX;
    int mY;
    int mZoomLevel;
    int mWidth;
    int mHeight;
    int mResolution;
    double originShift;
    double initialRes;
    QRectF mBoundingRect;
    QGraphicsPixmapItem* mItem;
};

#endif // LIPTILE_H
