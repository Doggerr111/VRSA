#include "lippostgisprovider.h"

LIPPostGisProvider::LIPPostGisProvider()
    : mConnectionFlag{false}
{
    mDb = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
}

bool LIPPostGisProvider::isConnected()
{
    return mConnectionFlag;
}

bool LIPPostGisProvider::isPostGIS()
{
    if (!isConnected())
        return false;
    QSqlQuery query;
    query.exec("SELECT extname FROM pg_extension WHERE extname = 'postgis'");
    if (query.next())
        return true;
    return false;

}

void LIPPostGisProvider::connect()
{
    readData();
    mDb->setDatabaseName(databaseName);
    mDb->setHostName(host);
    mDb->setUserName(userName);
    mDb->setPassword(password);
    mDb->setPort(port.toInt());

    if (mDb->isValid() && mDb->isDriverAvailable("QPSQL")) //если подключение успешное
        mConnectionFlag = true;
    else
    {
         QMessageBox::information(nullptr,"",mDb->lastError().text());
    }
    if (!mDb->open()) //если не подключились выводим сообщение об ошибке из бд
        mConnectionFlag=false;



}

void LIPPostGisProvider::getLayerNames()
{

}

GDALDataset* LIPPostGisProvider::readData()
{

    QString tempStr="PG:dbname="+databaseName+" host="+host+
                     " port="+port+" user="+userName+" password="+password;
    QByteArray tempBA = tempStr.toLocal8Bit();
    const char* dbInfo=tempBA.data();
    //qDebug()<< dbInfo;
    //const char* path ="PG:dbname=test host=localhost port=5432 user=postgres password=1234";
    //qDebug()<< path;
    GDALDataset *ds;
    ds = (GDALDataset *)GDALOpenEx(dbInfo,GDAL_OF_VECTOR | GDAL_OF_UPDATE,nullptr,nullptr,nullptr);

    return ds;

}


