#ifndef LIPCOORDINATESYSTEM_H
#define LIPCOORDINATESYSTEM_H
#include "gdal.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include <QString>
class LIPCoordinateSystem
{
public:
    LIPCoordinateSystem(/*QString name ="", QString projString=""*/);
    LIPCoordinateSystem(const LIPCoordinateSystem &other);
    ~LIPCoordinateSystem();
    bool setName(QString name);
    bool setProj(QString proj);
    bool isProjValid(QString proj);
    bool setOGRSpatialRef(OGRSpatialReference* ref);
    OGRSpatialReference *getOGRSpatialRef();
    //static LIPCoordinateSystem* fromOGR(OGRSpatialReference* ref);


    QString getName();
    const char *getProj();
private:
    QString mName;
    QString mProjString;
    OGRSpatialReference* mSR;


};

#endif // LIPCOORDINATESYSTEM_H
