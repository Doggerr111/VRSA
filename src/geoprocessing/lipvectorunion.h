#ifndef LIPVECTORUNION_H
#define LIPVECTORUNION_H
#include "vector/lipvectorconvertor.h"
#include "lippointlayer.h"
#include "lippolygonlayer.h"
#include "liplinelayer.h"
#include <geos/util/TopologyException.h>
#include <geos/util/Assert.h>
class LIPVectorUnion
{
public:
    LIPVectorUnion();
    static QVector<QVector<QPointF>> getUnion(LIPVectorLayer* inputLayer, LIPVectorLayer* overlayLayer);

private:
    LIPVectorLayer *resultLayer;
};

#endif // LIPVECTORUNION_H
