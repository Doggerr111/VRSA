#ifndef LIPVECTORTYPECHECKER_H
#define LIPVECTORTYPECHECKER_H
#include "lippolygonlayer.h"
#include "liplinelayer.h"
#include "lippointlayer.h"

class LIPVectorTypeChecker
{
public:
    LIPVectorTypeChecker();
    static bool isPointLayer(LIPVectorLayer* layer);
    static bool isLineLayer(LIPVectorLayer* layer);
    static bool isPolygonLayer(LIPVectorLayer* layer);

};

#endif // LIPVECTORTYPECHECKER_H
