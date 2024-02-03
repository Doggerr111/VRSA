#ifndef LIPVECTORLAYER_H
#define LIPVECTORLAYER_H
#include "liptypes.h"
class LIPVectorLayer
{
public:
    LIPVectorLayer();
private:
    LIPGeometryType geometryType;
    LIPTypes::LIPCrs crs;
    char* unicode;



};

#endif // LIPVECTORLAYER_H
