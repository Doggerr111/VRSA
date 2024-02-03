#include "liptriangle.h"



LIPTriangle::LIPTriangle()
    :isBad(false)
{
}

LIPTriangle::LIPTriangle(const QPointF a, const QPointF b, const QPointF c)
    :isBad(false), a(a), b(b), c(c)
{
    calculateCircumCircle();
}

bool LIPTriangle::operator==(const LIPTriangle &right) const
{
    return(a==right.getA() && b==right.getB() && c==right.getC());

}

//LIPTriangle::LIPTriangle(const LIPTriangle &copy)
//    :isBad(false), a(copy.a), b(copy.b), c(copy.c)
//{
//    calculateCircumCircle();
//}



//bool LIPTriangle::operator==(const LIPTriangle &right) const
//{
//    return isContainVertex(right.a) && isContainVertex(right.b) && isContainVertex(right.c);
//}

//LIPTriangle &LIPTriangle::operator=(const LIPTriangle &right)
//{
//    setVertex(right.a, right.b, right.c);
//    return *this;
//}

bool LIPTriangle::isContainEdge(const QPointF a, const QPointF b) const
{
    return isContainVertex(a) && isContainVertex(b);
}

bool LIPTriangle::isContainVertex(const QPointF a) const
{
    return this->a == a || this->b == a || this->c == a;
}

/*
 * 求获取三角形的外接圆，设圆心是(x, y)
 * 因为三角形三点都在圆上，得到两等式:
 * (ax-x)(ax-x) + (ay-y)(ay-y) = (bx-x)(bx-x) + (by-y)(by-y)
 * (ax-x)(ax-x) + (ay-y)(ay-y) = (cx-x)(cx-x) + (cy-y)(cy-y)
 * 化简后：
 * (2ax-2bx)(x) + (2ay-2by)(y) = (ax)(ax) - (bx)(bx) + (ay)(ay) - (by)(by)
 * (2ax-2cx)(x) + (2ay-2cy)(y) = (ax)(ax) - (cx)(cx) + (ay)(ay) - (cy)(cy)
 * 令:
 * A1 = (2ax-2bx), B1 = (2ay-2by), C1 = (ax)(ax) - (bx)(bx) + (ay)(ay) - (by)(by)
 * A2 = (2ax-2cx), B2 = (2ay-2cy), C2 = (ax)(ax) - (cx)(cx) + (ay)(ay) - (cy)(cy)
 * 并组成二阶行列式：
 * |A1 B1| |x|   |C1|
 * |     |*| | = |  |
 * |A2 B2| |y|   |C2|
 * 令:
 *     |A1 B1|
 * D = |     |
 *     |A2 B2|
 * 克拉默法则： xi = |Di| / |D|，其中Di是D的第i列被C替换的行列式。
 *
 * 故最终圆心(x, y)为：
 * x = ((C1*B2)-(C2*B1))/((A1*B2)-(A2*B1))
 * y = ((A1*C2)-(A2*C1))/((A1*B2)-(A2*B1))
 */
void LIPTriangle::calculateCircumCircle()
{
    double ax = a.x(), ay = a.y();
    double bx = b.x(), by = b.y();
    double cx = c.x(), cy = c.y();

    double A1 = 2*(ax-bx), B1 = 2*(ay-by), C1 = ax*ax - bx*bx + ay*ay - by*by;
    double A2 = 2*(ax-cx), B2 = 2*(ay-cy), C2 = ax*ax - cx*cx + ay*ay - cy*cy;

    double circleX = ((C1*B2)-(C2*B1))/((A1*B2)-(A2*B1));
    double circleY = ((A1*C2)-(A2*C1))/((A1*B2)-(A2*B1));
    QPointF center = QPointF(circleX, circleY);

//    qDebug() << "x=" << circleX << "y=" << circleY << "r=" << center.distance(a);
    circumcircle = LIPCircle(center, sqrt(pow(a.x()-center.x(), 2) + pow(a.y()-center.y(), 2)));
}
bool LIPTriangle::isVertexInCircumcircle(const QPointF ver)
{
    QPointF center=circumcircle.getCenter();
    return circumcircle.getRadis() >= sqrt(pow(center.x()-ver.x(), 2) + pow(center.y()-ver.y(), 2));
}

void LIPTriangle::setVertex(const QPointF a, const QPointF b, const QPointF c)
{
    this->a = a;
    this->b = b;
    this->c = c;
    calculateCircumCircle();
}
