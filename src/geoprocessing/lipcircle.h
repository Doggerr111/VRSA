#ifndef LIPCIRCLE_H
#define LIPCIRCLE_H
#include <QPointF>

class LIPCircle
{
public:
    LIPCircle();
    LIPCircle(QPointF center, double radis);
    LIPCircle &operator=(const LIPCircle &right);

    QPointF getCenter() {return center;}
    double getRadis() {return rad;}
private:
    QPointF center;
    double rad;
};

#endif // LIPCIRCLE_H
