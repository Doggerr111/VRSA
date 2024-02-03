#ifndef LIPTRIANGULATIONGEOS_H
#define LIPTRIANGULATIONGEOS_H
#include <geos/triangulate/DelaunayTriangulationBuilder.h>
#include <geos/triangulate/quadedge/QuadEdgeSubdivision.h>
#include "lippointlayer.h"
#include "lipwidgetmanager.h"
#include "lipvectorconvertor.h"
#include <geos/geom/GeometryCollection.h>
#include "liplayercreator.h"
//#include <geos/geom/>
class LIPTriangulationGeos
{
public:
    LIPTriangulationGeos();
    //
    static QVector<QVector<QPointF>> getTriangulation(LIPVectorLayer* pointLayer);


};

#endif // LIPTRIANGULATIONGEOS_H
