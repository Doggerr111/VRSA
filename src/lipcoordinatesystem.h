#ifndef LIPCOORDINATESYSTEM_H
#define LIPCOORDINATESYSTEM_H
#include "gdal.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include <QString>
class LIPCoordinateSystem: public OGRSpatialReference
{
public:
    LIPCoordinateSystem(/*QString name ="", QString projString=""*/);
    bool setName(QString name);
    bool setProj(QString proj);
    bool isProjValid(QString proj);
    static LIPCoordinateSystem* fromOGR(OGRSpatialReference* ref);


    QString getName();
    const char *getProj();
private:
    QString mName;
    QString mProjString;


};

#endif // LIPCOORDINATESYSTEM_H
