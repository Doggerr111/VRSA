#ifndef LIPTRIANGLE_H
#define LIPTRIANGLE_H
#include <QPointF>
#include <vector/lippoint.h>
#include <lipcircle.h>
#include<cmath>
class LIPTriangle
{
public:
    LIPTriangle();
    LIPTriangle(QPointF a, QPointF b, QPointF c);
    LIPTriangle(LIPPoint* a, LIPPoint* b, QPointF* c);
    bool operator==(const LIPTriangle &right) const;
    bool isVertexInCircumcircle(QPointF ver);
    bool isContainEdge(QPointF a, QPointF b) const;
    bool isContainVertex(const QPointF a) const;
    void setVertex(QPointF a, const QPointF b, QPointF c);

    QPointF getA() const {return a;}
    QPointF getB() const {return b;}
    QPointF getC() const {return c;}
    LIPCircle getCircumcircle() {return circumcircle;}

    bool isBad;
private:
    void calculateCircumCircle();
    QPointF a;
    QPointF b;
    QPointF c;

    LIPCircle circumcircle;
};

#endif // LIPTRIANGLE_H
