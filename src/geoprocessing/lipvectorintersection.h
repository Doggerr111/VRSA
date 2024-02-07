#ifndef LIPVECTORINTERSECTION_H
#define LIPVECTORINTERSECTION_H
#include "vector/lipvectorconvertor.h"
#include "lippointlayer.h"
#include "lippolygonlayer.h"
#include "liplinelayer.h"
#include <geos/util/TopologyException.h>
#include <geos/util/Assert.h>
class LIPVectorIntersection
{
public:
    LIPVectorIntersection();
    static QVector<QVector<QPointF>> getIntersection(LIPVectorLayer* inputLayer, LIPPolygonLayer* overlayLayer);

private:
    LIPVectorLayer *resultLayer;
};

#endif // LIPVECTORINTERSECTION_H
