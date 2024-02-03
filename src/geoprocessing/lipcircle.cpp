#include "lipcircle.h"

LIPCircle::LIPCircle()
{

}

LIPCircle::LIPCircle(QPointF center, double radis)
    :center(center), rad(radis)
{

}

LIPCircle &LIPCircle::operator=(const LIPCircle &right)
{
    center = right.center;
    rad = right.rad;
    return *this;
}
