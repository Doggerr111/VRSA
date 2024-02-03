#include "lipmultipoint.h"
#include <QDebug>
LIPMultiPoint::LIPMultiPoint()
    :MultiPoint{nullptr}
{

}

bool LIPMultiPoint::AddPoint(LIPPoint *new_point)
{
    if (new_point!=nullptr)
    {
        MultiPoint.append(new_point);
        return true;
    }
    else {
        return false;
        qDebug()<<"Error: cant add nullptr point";

    }
}

bool LIPMultiPoint::SetPoints(QVector<LIPPoint *> points)
{
    if (!points.isEmpty())
    {
        MultiPoint=points;
        return true;
    }
    else {
        return false;
        qDebug()<<"Error: cant add empty points vector";

    }
}

LIPPoint *LIPMultiPoint::PointAt(int id)
{
    if (id>0 && id<MultiPoint.size())
    {
        return MultiPoint.at(id);
    }
    else {
        qDebug()<<"Error: cant return point at "+QString::number(id)+". Point with that id doesn't exist";
        return nullptr;
    }
}

QVector<LIPPoint *> LIPMultiPoint::GetPoints()
{
    return MultiPoint;
}
