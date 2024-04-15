#include "lipxyzconnection.h"
#include <QMessageBox>
LIPXYZConnection::LIPXYZConnection()
{
    mNetworkManager = new QNetworkAccessManager(this);
    connect(mNetworkManager, &QNetworkAccessManager::finished, this, &LIPXYZConnection::onReplyFinished);

//    loadTile("https://tile.memomaps.de/tilegen/0/0/0.png", 0, 0);

//    LIPTile *tile = new LIPTile(0,0,0);
//    vect.append(tile);
//    loadTile("https://tile.memomaps.de/tilegen/1/1/1.png", 0, 0);
//    LIPTile *tile1 = new LIPTile(1,1,1);
//    vect.append(tile1);
//    loadTile("https://tile.memomaps.de/tilegen/1/0/0.png", 0, 0);
//    LIPTile *tile2 = new LIPTile(1,0,0);
//    vect.append(tile2);

//    loadTile("https://tile.memomaps.de/tilegen/1/1/0.png", 0, 0);
//    LIPTile *tile3 = new LIPTile(1,1,0);
//    vect.append(tile3);
//    scaleRatio = {{0,2},{1,2}};

    scales = { 500, 1000, 2000, 4000, 8000, 15000, 35000, 70000, 150000, 250000, 500000, 1000000, 2000000,
                                     4000000, 10000000, 15000000, 35000000, 70000000, 150000000, 250000000, 500000000 };

    QString request  = "https://tile.memomaps.de/tilegen/";
    request.append(QString::number(13) + "/");
    request.append(QString::number(5434) + "/");
    request.append(QString::number(2362) + ".png");

    loadTile(request, 0, 0);
}

void LIPXYZConnection::test()
{

}

void LIPXYZConnection::loadTile(const QString& url, qreal x, qreal y)
{
    QNetworkRequest request(url);
    mNetworkManager->get(request);
}

void LIPXYZConnection::onReplyFinished(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();
        QPixmap pixmap;
        QString url = reply->url().toString();
        //LIPTile *tile = new LIPTile(zoomLevel,x,y);

        pixmap.loadFromData(data);
        //pixmap.save("idk.png");
        if (pixmap.isNull())
            QMessageBox::warning(nullptr,"","");
        QRegExp rx("\\d+");

        int count = 0;
        int z, x, y;
        int pos = 0;
        while ((pos = rx.indexIn(url, pos)) != -1 && count < 3) {
            QString digits = rx.cap(0);
            if (count == 0)
            {
                z = digits.toInt(); // Преобразуем первую цифру в int и сохраняем в z
            } else if (count == 1) {
                x = digits.toInt(); // Преобразуем вторую цифру в int и сохраняем в x
            } else if (count == 2) {
                y = digits.toInt(); // Преобразуем третью цифру в int и сохраняем в y
            }
            count++;
            pos += rx.matchedLength();
        }

        LIPTile *tile = new LIPTile(z,x,y);
        tile->setGraphicsItem(pixmap);
        vect.append(tile);
        //m_currentTile->setPixmap(pixmap);
    }
    else
    {
        qDebug()<<reply->errorString();
    }

    reply->deleteLater();
    //delete reply;
}

void LIPXYZConnection::onViewportChanged(int scale, QRectF visibleRect)
{
    //определяем уровень зума
    for (std::size_t i = 0; i < scales.size(); ++i)
    {
        if (scale >= scales[i])
            zoomLevel = 20-i;
        else
            break;
    }


//    if (scale >= 500000000)
//            zoomLevel = 0;
//        else if (scale >= 250000000)
//            zoomLevel = 1;
//        else if (scale >= 150000000)
//            zoomLevel = 2;
//        else if (scale >= 70000000)
//            zoomLevel = 3;
//        else if (scale >= 35000000)
//            zoomLevel = 4;
//        else if (scale >= 15000000)
//            zoomLevel = 5;
//        else if (scale >= 10000000)
//            zoomLevel = 6;
//        else if (scale >= 4000000)
//            zoomLevel = 7;
//        else if (scale >= 2000000)
//            zoomLevel = 8;
//        else if (scale >= 1000000)
//            zoomLevel = 9;
//        else if (scale >= 500000)
//            zoomLevel = 10;
//        else if (scale >= 250000)
//            zoomLevel = 11;
//        else if (scale >= 150000)
//            zoomLevel = 12;
//        else if (scale >= 70000)
//            zoomLevel = 13;
//        else if (scale >= 35000)
//            zoomLevel = 14;
//        else if (scale >= 15000)
//            zoomLevel = 15;
//        else if (scale >= 8000)
//            zoomLevel = 16;
//        else if (scale >= 4000)
//            zoomLevel = 17;
//        else if (scale >= 2000)
//            zoomLevel = 18;
//        else if (scale >= 1000)
//            zoomLevel = 19;
//        else if (scale >= 500)
//            zoomLevel = 20;

    //zoomLevel = 20 - zoomLevel;
    double topLat;
    double leftLon;
    double bottomLat;
    double rightLon;
    visibleRect.getRect(&topLat, &leftLon, &bottomLat, &rightLon);
    bottomLat = topLat + bottomLat;
    rightLon = leftLon - rightLon;
    qDebug()<<topLat <<leftLon<<bottomLat<<rightLon;
    QPoint minXY = metersToTile(QPointF(topLat, leftLon));
    QPoint maxXY = metersToTile(QPointF(bottomLat, rightLon));
    for (auto tile: vect)
    {
        delete tile;
    }
    vect.clear();
    counter=0;
    for (int x = minXY.x(); x<=maxXY.x(); x++)
    {
        for (int y = maxXY.y(); y<=minXY.y(); y++)
        {
            QString request  = "https://tile.memomaps.de/tilegen/";
            request.append(QString::number(zoomLevel) + "/");

//            tx, (2**zoom - 1) - ty
            request.append(QString::number(x) + "/");
            request.append(QString::number((std::pow(2,zoomLevel)-1)-y) + ".png");

//            LIPTile *tile = new LIPTile(zoomLevel,x,y);
//            vect.append(tile);
            loadTile(request, 0, 0);
            qDebug()<<request;
        }
    }

}

QPoint LIPXYZConnection::metersToTile(QPointF coord)
{
    double n = pow(2.0, zoomLevel);
    double res = 2*M_PI*6378137/(256*std::pow(2, zoomLevel));
    double intialRes= (2 * M_PI * 6378137)/256;
    res = intialRes/std::pow(2,zoomLevel);

    double originShift = 2 * M_PI * 6378137/2;
    int x = static_cast<int>((originShift+256*coord.x())/(res));
    int y = static_cast<int>((originShift+256*coord.y())/(res));


    int x3 = static_cast<int>((originShift+coord.x())/(256*res));
    int y3 = static_cast<int>((originShift+coord.y())/(256*res));

    double equator = 40075016.68557849;
    int x2 = static_cast<int>(coord.x() + (equator / 2.0)) / (equator / 256.0);
    int y2 = static_cast<int>((coord.y() - (equator / 2.0)) / (equator / -256.0));


    qDebug()<<x3<<y3;

//    int x_tile = static_cast<int>((coord.x() + 180.0) / 360.0 * n);
//    int y_tile = static_cast<int>((1.0 - log(tan(coord.y() * M_PI / 180.0))));
    return QPoint(x3, y3);
}


