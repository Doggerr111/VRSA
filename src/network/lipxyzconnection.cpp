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

    //    QString request  = "https://tile.memomaps.de/tilegen/";
    //    request.append(QString::number(13) + "/");
    //    request.append(QString::number(5434) + "/");
    //    request.append(QString::number(2362) + ".png");

    //    loadTile(request, 0, 0);

    //    QString request1  = "https://tile.memomaps.de/tilegen/";
    //    request1.append(QString::number(1) + "/");
    //    request1.append(QString::number(1) + "/");
    //    request1.append(QString::number(1) + ".png");

    //    loadTile(request1, 0, 0);

    //    QString request2  = "https://tile.memomaps.de/tilegen/";
    //    request2.append(QString::number(1) + "/");
    //    request2.append(QString::number(0) + "/");
    //    request2.append(QString::number(1) + ".png");

    //    loadTile(request2, 0, 0);

    //    QString request3  = "https://tile.memomaps.de/tilegen/";
    //    request3.append(QString::number(1) + "/");
    //    request3.append(QString::number(0) + "/");
    //    request3.append(QString::number(0) + ".png");

    //    loadTile(request3, 0, 0);

//    QString request4  = "https://tile.memomaps.de/tilegen/";
//    request4.append(QString::number(1) + "/");
//    request4.append(QString::number(1) + "/");
//    request4.append(QString::number(0) + ".png");

//    loadTile(request4, 0, 0);
}

void LIPXYZConnection::test()
{

}

void LIPXYZConnection::loadTile(const QString& url, qreal x, qreal y)
{
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Mozilla/5.0");
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
    int prevZoom = zoomLevel;

    //определяем уровень зума
    for (std::size_t i = 0; i < scales.size(); ++i)
    {
        if (scale >= scales[i])
            zoomLevel = 20-i;
        else
            break;
    }

    if (prevZoom!=zoomLevel)
    {
        for (auto tile: vect)
        {
            delete tile;
        }
        vect.clear();
    }
    mNetworkManager->clearAccessCache();


    double topLat;
    double leftLon;
    double bottomLat;
    double rightLon;
    visibleRect.getRect(&topLat, &leftLon, &bottomLat, &rightLon);
//    bottomLat = topLat + bottomLat;
//    rightLon = leftLon - rightLon;
    qDebug()<<"extent"<<QString::number(topLat,'f',3)<<QString::number(leftLon,'f',3)
           <<QString::number(bottomLat,'f',3)<<QString::number(rightLon,'f',3);
    QPoint minXY = metersToTile(QPointF(topLat, leftLon));
    QPoint maxXY = metersToTile(QPointF(bottomLat, rightLon));
    if (zoomLevel<2)
        checkMinMaxXY(minXY,maxXY);

    counter=0;

    QVector<LIPTile*> tempVec; //= vect;

    for (int x = minXY.x(); x<=maxXY.x(); x++)
    {
        for (int y = maxXY.y(); y<=minXY.y(); y++)
        {
            int tempZ, tempX, tempY;
            bool flag=true;
            for (LIPTile* tile: vect)
            {
                tile->getZoomXY(tempZ, tempX, tempY);
                if (tempZ==zoomLevel && tempX == x && tempY == (std::pow(2,zoomLevel) - 1) - y)
                {
                    tempVec.append(tile);
                    flag=false;
                    break;
                }
            }
            if (flag)
            {
                QString request  = "https://tile.memomaps.de/tilegen/";
                request.append(QString::number(zoomLevel) + "/");
                request.append(QString::number(x) + "/");
                request.append(QString::number((std::pow(2,zoomLevel) - 1) - y) + ".png");
                loadTile(request, 0, 0);
                qDebug()<<request;
            }

        }
    }


    for (LIPTile* tile: vect)
    {
        if (!tempVec.contains(tile))
        {
            delete tile;
            tile=nullptr;
        }
    }
    vect.clear();
    vect=tempVec;

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

bool LIPXYZConnection::setZoomLevel(int zoom)
{
    zoomLevel=zoom;
}

void LIPXYZConnection::checkMinMaxXY(QPoint &minXY, QPoint &maxXY)
{

    int pow = 1<<zoomLevel;
    if (minXY.x()<0)
        minXY.setX(0);
    if (minXY.y()<0)
        minXY.setY(0);
    if (minXY.x()>=pow)
        minXY.setX(pow-1);
    if (minXY.y()>=pow)
        minXY.setY(pow-1);

    if (maxXY.x()<0)
        maxXY.setX(0);
    if (maxXY.y()<0)
        maxXY.setY(0);
    if (maxXY.x()>=pow)
        maxXY.setX(pow-1);
    if (maxXY.y()>=pow)
        maxXY.setY(pow-1);

//    if (minXY.x()>=(pow))
//        minXY.setX(pow-1);
//    if (minXY.y()>=pow)
//        minXY.setY(pow-1);

}


