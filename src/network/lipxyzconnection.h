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
    LIPXYZConnection(QString URL, int minZoom, int maxZoom, QString name = "xyz tiles");
    ~LIPXYZConnection();
    void test();
    void loadTile(const QString& url, qreal x, qreal y);
    /** pseudo-merkator to tile pixels */
    QPoint metersToTile(QPointF coord);
    bool setZoomLevel(int zoom);
    void checkMinMaxXY(QPoint &minXY, QPoint &maxXY);

    QString getName();
    QString getURL();

    void setZValue(int value);
    void setVisible(bool fl);
public slots:
    void onReplyFinished(QNetworkReply *reply);
    void onViewportChanged(int scale, QRectF visibleRect);


private:
    QNetworkAccessManager* mNetworkManager;
    QVector<LIPTile*> mTiles;
    int mZoomLevel;
    QString mURL;
    QString fullURL;
    QString mName;
    //std::map<int, int> scaleRatio; //key - zoom level, value - scale (with 96.0 dpi)
    std::vector<int> mScales;
    int mMinZoom;
    int mMaxZoom;

};

#endif // LIPXYZCONNECTION_H
