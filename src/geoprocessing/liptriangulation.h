#ifndef LIPTRIANGULATION_H
#define LIPTRIANGULATION_H
#include <QPointF>
#include <QVector>
#include <QPolygonF>
#include "liptriangle.h"
#include "lippointlayer.h"
#include <QLineF>
#include <vector>
#include <lipedge.h>

class LIPTriangulation: QObject
{
public:
    LIPTriangulation();

    LIPTriangle createSuperTriangle(const QVector<QPointF> &inputs); // create the super triangle.
    void addPoint(const QPointF &vec);
    QVector<LIPTriangle> getTriangulation();

    void setDelegate(void* d);
private:
    void *delegate;
    LIPTriangle *super;

    QVector<LIPTriangle> triangulation;

};
//QVector<LIPTriangle> triangulation;
#endif // LIPTRIANGULATION_H



QVector<QPointF> triangulate(LIPPointLayer* layer);

QPair<QVector<LIPEdge>, QVector<LIPTriangle>> generateVoronoiDiagramm(LIPPointLayer* layer);

QVector<LIPCircle> getCircumCircles();

QVector<QPair<LIPCircle, LIPCircle>> GetPair();
