#ifndef LIPXYZCONNECTION_H
#define LIPXYZCONNECTION_H
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include "liptile.h"
class LIPXYZConnection: public QObject
{
     Q_OBJECT
public:
    LIPXYZConnection();
    void test();
    void loadTile(const QString& url, qreal x, qreal y);
public slots:
    void onReplyFinished(QNetworkReply *reply);
    void onViewportChanged(int scale, QRectF visibleRect);


private:
    /** pseudo-merkator to tile pixels */
    QPoint metersToTile(QPointF coord);

    QNetworkAccessManager* mNetworkManager;
    QVector<LIPTile*> vect;
    int counter=0;
    int zoomLevel;
    std::map<int, int> scaleRatio; //key - zoom level, value - scale (with 96.0 dpi)
    std::vector<int> scales;


};

#endif // LIPXYZCONNECTION_H
