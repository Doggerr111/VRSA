#include "lipcoordinatesystemlibrary.h"

LIPCoordinateSystemLibrary::LIPCoordinateSystemLibrary()
{
    LIPCoordinateSystem *WGS84 = new LIPCoordinateSystem();
    WGS84->setProj("+proj=longlat +datum=WGS84 +no_defs +type=crs");
    WGS84->setName("WGS 84");
    mCRSLib.append(WGS84);

    LIPCoordinateSystem *GSK_2011 = new LIPCoordinateSystem();
    GSK_2011->setProj("+proj=longlat +ellps=GSK2011 +no_defs +type=crs");
    GSK_2011->setName("ГСК-2011");
    mCRSLib.append(GSK_2011);

    LIPCoordinateSystem *PZ_90_11 = new LIPCoordinateSystem();
    PZ_90_11->setProj("+proj=longlat +a=6378136 +rf=298.257839303 +no_defs +type=crs");
    PZ_90_11->setName("ПЗ-90.11");
    mCRSLib.append(PZ_90_11);

    LIPCoordinateSystem *MSK_50_2 = new LIPCoordinateSystem();
    MSK_50_2->setProj("+proj=tmerc +lat_0=0 +lon_0=38.48333333333 +k=1 +x_0=2250000 +y_0=-5712900.566 +ellps=krass +towgs84=23.57,-140.95,-79.8,0,0.35,0.79,-0.22 +units=m +no_defs");
    MSK_50_2->setName("МСК-50 Зона 2");
    mCRSLib.append(MSK_50_2);

}

QVector<LIPCoordinateSystem*> LIPCoordinateSystemLibrary::getCRSLib()
{
    return mCRSLib;
}

LIPCoordinateSystem *LIPCoordinateSystemLibrary::getCRSbyName(QString CRSName)
{
    foreach(LIPCoordinateSystem *crs, mCRSLib)
    {
        if (crs->getName()==CRSName)
        {
            return crs;
        }
    }
    return nullptr;
}

