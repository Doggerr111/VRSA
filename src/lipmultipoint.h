#ifndef LIPMULTIPOINT_H
#define LIPMULTIPOINT_H

#include <QPointF>
#include <QObject>
#include <vector/lippoint.h>

class LIPMultiPoint
{

//    struct MultiPoint{
//        LIPPoint point;

//    };

public:
    LIPMultiPoint();
    bool AddPoint(LIPPoint* new_point);
    bool SetPoints(QVector<LIPPoint*> points);

    LIPPoint* PointAt(int id);
    QVector<LIPPoint*> GetPoints();

private:
    QVector<LIPPoint*> MultiPoint;
};

#endif // LIPMULTIPOINT_H
