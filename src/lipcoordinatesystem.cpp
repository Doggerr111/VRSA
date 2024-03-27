#include "lipcoordinatesystem.h"
#include <QDebug>
LIPCoordinateSystem::LIPCoordinateSystem()
    :mSR{nullptr}
{
    mSR = new OGRSpatialReference;
}

LIPCoordinateSystem::~LIPCoordinateSystem()
{
    delete mSR;
}

bool LIPCoordinateSystem::setName(QString name)
{
    mName=name;
}

bool LIPCoordinateSystem::setProj(QString proj)
{
    if (proj.isEmpty())
        return false;
    mProjString=proj;
    QByteArray ba = proj.toLocal8Bit();
    const char *projChar = ba.data();
    if (mSR->importFromProj4(projChar) == OGRERR_NONE)
    {
        qDebug()<<"import ok";
        return true;


    }
    else
    {
        qDebug()<<"import not ok";
        return false;

    }
}

bool LIPCoordinateSystem::isProjValid(QString proj)
{

}

bool LIPCoordinateSystem::setOGRSpatialRef(OGRSpatialReference *ref)
{
    if (ref==nullptr)
        return false;

    mSR=ref;
    return true;
}

OGRSpatialReference *LIPCoordinateSystem::getOGRSpatialRef()
{
    return mSR;
}

//LIPCoordinateSystem* LIPCoordinateSystem::fromOGR(OGRSpatialReference* ref)
//{
////    LIPCoordinateSystem* newCRS = static_cast<LIPCoordinateSystem*>(ref);
////    newCRS->setName(ref->GetName());
////    return newCRS;
////    //newCRS->setProj(ref->GetProjParm())
//}

QString LIPCoordinateSystem::getName()
{
    return mName;
}

const char *LIPCoordinateSystem::getProj()
{
    return mProjString.toUtf8().constData();
}
