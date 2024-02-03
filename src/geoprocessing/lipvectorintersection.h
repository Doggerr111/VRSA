#ifndef LIPVECTORINTERSECTION_H
#define LIPVECTORINTERSECTION_H
#include "vector/lipvectorconvertor.h"
#include "lippointlayer.h"
#include "lippolygonlayer.h"
#include "liplinelayer.h"
class LIPVectorIntersection
{
public:
    LIPVectorIntersection();
    static LIPVectorLayer *getIntersection(LIPVectorLayer* inputLayer, LIPPolygonLayer* overlayLayer);

private:
    LIPVectorLayer *resultLayer;
};

#endif // LIPVECTORINTERSECTION_H
