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
    static LIPGeometryType getType(LIPVectorLayer* layer);
    static bool isSameType(LIPVectorLayer* layer1, LIPVectorLayer* layer2);

};

#endif // LIPVECTORTYPECHECKER_H
