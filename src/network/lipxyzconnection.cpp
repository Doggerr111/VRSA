#include "lipxyzconnection.h"
#include <QMessageBox>
LIPXYZConnection::LIPXYZConnection(QString URL, int minZoom, int maxZoom, QString name)
    : mURL{URL},
      fullURL{URL},
      mName{name},
      mMinZoom{minZoom},
      mMaxZoom{maxZoom}
{
    mURL = mURL.split("{z}")[0]; //получаем строку до символа {x}
    mNetworkManager = new QNetworkAccessManager(this);
    connect(mNetworkManager, &QNetworkAccessManager::finished, this, &LIPXYZConnection::onReplyFinished);
    //21 уровень масштабирования
    mScales = { 500, 1000, 2000, 4000, 8000, 15000, 35000, 70000, 150000, 250000, 500000, 1000000, 2000000,
                                     4000000, 10000000, 15000000, 35000000, 70000000, 150000000, 250000000, 500000000 };
}

LIPXYZConnection::~LIPXYZConnection()
{
    delete mNetworkManager;
    for (auto tile: mTiles)
    {
        delete tile;
    }
    mTiles.clear();
}

void LIPXYZConnection::test()
{

}

void LIPXYZConnection::loadTile(const QString& url, qreal x, qreal y)
{
    QNetworkRequest request(url);
    //request.setRawHeader("User-Agent", "Mozilla/5.0");
    request.setHeader(QNetworkRequest::UserAgentHeader, "vrsa/%1/%2");
    request.setRawHeader("Accept-Language", "en-US,en;q=0.9,es;q=0.8,de;q=0.7");
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
        mTiles.append(tile);
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
    int prevZoom = mZoomLevel;

    //определяем уровень зума
    for (std::size_t i = 0; i < mScales.size(); ++i)
    {
        if (scale >= mScales[i])
            mZoomLevel = 20-i;
        else
            break;
    }

    if (mZoomLevel < mMinZoom)
        mZoomLevel = mMinZoom;
    if (mZoomLevel > mMaxZoom)
        mZoomLevel = mMaxZoom;
    if (prevZoom!=mZoomLevel)
    {
        for (auto tile: mTiles)
        {
            delete tile;
        }
        mTiles.clear();
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
    if (mZoomLevel<2)
        checkMinMaxXY(minXY,maxXY);

    QVector<LIPTile*> tempVec; //= vect;

    for (int x = minXY.x(); x<=maxXY.x(); x++)
    {
        for (int y = maxXY.y(); y<=minXY.y(); y++)
        {
            int tempZ, tempX, tempY;
            bool flag=true;
            for (LIPTile* tile: mTiles)
            {
                tile->getZoomXY(tempZ, tempX, tempY);
                if (tempZ==mZoomLevel && tempX == x && tempY == (std::pow(2,mZoomLevel) - 1) - y)
                {
                    tempVec.append(tile);
                    flag=false;
                    break;
                }
            }
            if (flag)
            {
                QString request  = mURL;
                request.append(QString::number(mZoomLevel) + "/");
                request.append(QString::number(x) + "/");
                request.append(QString::number((std::pow(2,mZoomLevel) - 1) - y) + ".png");
                loadTile(request, 0, 0);
                qDebug()<<request;

            }

        }
    }


    for (LIPTile* tile: mTiles)
    {
        if (!tempVec.contains(tile))
        {
            delete tile;
            tile=nullptr;
        }
    }
    mTiles.clear();
    mTiles=tempVec;

}

QPoint LIPXYZConnection::metersToTile(QPointF coord)
{
    double n = pow(2.0, mZoomLevel);
    double res = 2*M_PI*6378137/(256*std::pow(2, mZoomLevel));
    double intialRes= (2 * M_PI * 6378137)/256;
    res = intialRes/std::pow(2, mZoomLevel);

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
    mZoomLevel=zoom;
}

void LIPXYZConnection::checkMinMaxXY(QPoint &minXY, QPoint &maxXY)
{

    int pow = 1<<mZoomLevel;
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

QString LIPXYZConnection::getName()
{
    return mName;
}

QString LIPXYZConnection::getURL()
{
    return fullURL;
}

void LIPXYZConnection::setZValue(int value)
{
    for (LIPTile* tile: mTiles)
    {
        QGraphicsPixmapItem* item = tile->getItem();
        if (item)
            item->setZValue(value);
    }
}

void LIPXYZConnection::setVisible(bool fl)
{
    for (LIPTile* tile: mTiles)
    {
        QGraphicsPixmapItem* item = tile->getItem();
        if (item)
            item->setVisible(fl);
    }
}


