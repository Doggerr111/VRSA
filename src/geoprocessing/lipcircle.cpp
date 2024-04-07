#include "lipcircle.h"

LIPCircle::LIPCircle()
{

}

LIPCircle::LIPCircle(QPointF center, double radis)
    :center(center), rad(radis)
{

}

LIPCircle::LIPCircle(const LIPCircle &circle)
{
    center=circle.center;
    rad=circle.rad;
}

LIPCircle &LIPCircle::operator=(const LIPCircle &right)
{
    center = right.center;
    rad = right.rad;
    return *this;
}
