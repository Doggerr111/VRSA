#include "liptriangulation.h"

LIPTriangulation::LIPTriangulation()
{
}

void LIPTriangulation::addPoint(const QPointF &vec)
{
//    qDebug() << "-------- add new point:";
//    qDebug() << "waiting triangulation size " << triangulation.size();

    // get all the bad triangles.
    QVector<LIPEdge> polygon;
    int basTriagles = 0;
    for (auto triIter = triangulation.begin(); triIter != triangulation.end(); triIter++)
    {
        if (triIter->isVertexInCircumcircle(vec))
        {
            basTriagles++;
            triIter->isBad = true;
            polygon.push_back(QLineF(triIter->getA(), triIter->getB()));
            polygon.push_back(QLineF(triIter->getB(), triIter->getC()));
            polygon.push_back(QLineF(triIter->getA(), triIter->getC()));
        }
        else
        {
            triIter->isBad = false;
        }
    }
//    qDebug() << "bad triangles size: " << basTriagles;
//    qDebug() << "waiting edges size: " << polygon.size();

    // remove bad triangles from triangulations.
    triangulation.erase(std::remove_if(triangulation.begin(), triangulation.end(), [](LIPTriangle &tri){
                            return tri.isBad;
                        }), triangulation.end());

//    qDebug() << "current triangulation size " << triangulation.size();
//    int rhha = 0;
//    for (auto triIter = triangulation.begin(); triIter != triangulation.end(); triIter++)
//    {
//        if (triIter->isBad)
//        {
//            qDebug() << "е·Із»Џeraseдє†пјЊдёєе•Ґиїжњ‰isBad" << rhha;
//        }
//        rhha++;
//    }
    // find remove bad edges in polygon
    for (auto egIter = polygon.begin(); egIter != polygon.end(); egIter++)
    {
        for (auto egIter2 = egIter+1; egIter2 != polygon.end(); egIter2++)
        {
            if ((*egIter) == (*egIter2))
            {
                egIter->isBad = true;
                egIter2->isBad = true;
            }
        }
    }
    polygon.erase(std::remove_if(polygon.begin(), polygon.end(), [](LIPEdge &e){
                      return e.isBad;
                  }), polygon.end());
//    qDebug() << "good egdes size: " << polygon.size();

    // add the new point in polygon, and form new triangles.
    for (auto egIter = polygon.begin(); egIter != polygon.end(); egIter++)
    {
        LIPTriangle newTri(egIter->p1(), egIter->p2(), vec);
        triangulation.push_back(newTri);
    }
//    qDebug() << "latest triangulation size " << triangulation.size()<< endl;

    for (auto triIter = triangulation.begin(); triIter != triangulation.end(); triIter++)
    {
        if (delegate)
        {
//            MainWindow *main = static_cast<MainWindow*>(delegate);
//            main->addLine(triIter->getA().convertToQPoint(), triIter->getB().convertToQPoint());
//            main->addLine(triIter->getB().convertToQPoint(), triIter->getC().convertToQPoint());
//            main->addLine(triIter->getA().convertToQPoint(), triIter->getC().convertToQPoint());
//            main->addCircle(triIter->getCircumcircle().getCenter().convertToQPoint(), triIter->getCircumcircle().getRadis());
        }
    }
}

QVector<LIPTriangle> LIPTriangulation::getTriangulation()
{
    // remove all the triangles contains vertexs in super triangle.
    triangulation.erase(std::remove_if(triangulation.begin(), triangulation.end(), [this](LIPTriangle &tri){
                            return tri.isContainVertex(super->getA()) || tri.isContainVertex(super->getB()) || tri.isContainVertex(super->getC());
                        }), triangulation.end());
//    qDebug() << "final triangulation size " << triangulation.size();
    return triangulation;
}

LIPTriangle LIPTriangulation::createSuperTriangle(const QVector<QPointF> &inputs)
{
    //if (super)
        //delete super;

    // get minX minY maxX maxY of input vertexs.
    double minX = inputs[0].x();
    double minY = inputs[0].y();
    double maxX = minX, maxY = minY;
    for (auto iter = inputs.begin(); iter != inputs.end(); iter++)
    {
        if (iter->x() < minX) minX = iter->x();
        if (iter->y() < minY) minY = iter->y();
        if (iter->x() > maxX) maxX = iter->x();
        if (iter->y() > maxY) maxY = iter->y();
    }

//    qDebug() << "super is " << minX << ", " << minY << " ==> " << maxX << ", " << maxY << endl;

    // create an isosceles triangle that is large enough.
    double delta = 50;
    double height = 2*delta + maxY-minY;
    double width = height * (maxX-minX) / delta;

    super = new LIPTriangle(QPointF(minX-width/2, minY-delta),
                 QPointF(maxX+width/2, minY-delta),
                 QPointF((maxX+minX)/2, maxY+delta));

    triangulation.clear();
    triangulation.push_back(*super);

    return *super;
}

void LIPTriangulation::setDelegate(void *d)
{
    delegate = d;
}

QVector<QPointF> triangulate(LIPPointLayer *layer)
{
    LIPTriangulation tr;
    QRectF bBox=layer->getBoundingBox();
    QVector<QPointF> points;
    for (QPointF* pointer : layer->returnCords()) {
            if (pointer) {
                LIPPoint value;

                value.setX(pointer->x());
                value.setY(pointer->y());
                points.append(value);
            }
        }
    tr.createSuperTriangle(points); //
    for (auto iter = points.begin(); iter != points.end(); iter++)
    {
        tr.addPoint(*iter);
    }
    QVector<LIPTriangle> ret = tr.getTriangulation(); //all triangles
    QVector<LIPEdge> interTrignales;
    //voronoi
//    foreach(LIPTriangle triangle, ret)
//    {
//        //getting edges
//        LIPEdge ab(QLineF(triangle.getA(), triangle.getB()));
//        LIPEdge bc(QLineF(triangle.getA(), triangle.getB()));
//        LIPEdge ac(QLineF(triangle.getA(), triangle.getC()));
//        foreach(LIPTriangle intersTriangle, ret)
//        {
//            if (intersTriangle==triangle)
//                continue;
////            LIPEdge abInter(QLineF(intersTriangle.getA(), intersTriangle.getB()));
////            LIPEdge bcInter(QLineF(intersTriangle.getA(), intersTriangle.getB()));
////            LIPEdge acInter(QLineF(intersTriangle.getA(), intersTriangle.getC()));
//            //находим треугольники с общими гранями
//            if (triangle.isContainEdge(intersTriangle.getA(), intersTriangle.getB())
//                    || triangle.isContainEdge(intersTriangle.getB(), intersTriangle.getC())
//                    || triangle.isContainEdge(intersTriangle.getA(), intersTriangle.getC()))
//            {
//                QPointF centerTriangle=triangle.getCircumcircle().getCenter();
//                QPointF centerInterTriangle=intersTriangle.getCircumcircle().getCenter();
//                interTrignales.append(LIPEdge(QLineF(centerTriangle, centerInterTriangle)));
//            }
//        }

//    }


    QVector<QPointF> p;

    for (auto iter = ret.begin(); iter != ret.end(); iter++)
    {
        p.append(iter->getA());
        p.append(iter->getB());
        p.append(iter->getC());
    }
    return p; //or p
}
QVector<LIPCircle> tr_vecs;
QVector<QPair<LIPCircle, LIPCircle>> pairVec;
QPair<QVector<LIPEdge>, QVector<LIPTriangle>> generateVoronoiDiagramm(LIPPointLayer *layer)
{
    LIPTriangulation tr;
    QRectF bBox=layer->getBoundingBox();
    QVector<QPointF> points;
    for (QPointF* pointer : layer->returnCords()) {
            if (pointer) {
                LIPPoint value;

                value.setX(pointer->x());
                value.setY(pointer->y());
                points.append(value);
            }
        }
    tr.createSuperTriangle(points); //
    for (auto iter = points.begin(); iter != points.end(); iter++)
    {
        tr.addPoint(*iter);
    }
    QVector<LIPTriangle> ret = tr.getTriangulation(); //all triangles
    QVector<LIPEdge> interTrignales;
    //voronoi
    foreach(LIPTriangle triangle, ret)
    {
        //getting edges
        LIPEdge ab(QLineF(triangle.getA(), triangle.getB()));
        LIPEdge bc(QLineF(triangle.getA(), triangle.getB()));
        LIPEdge ac(QLineF(triangle.getA(), triangle.getC()));
        foreach(LIPTriangle intersTriangle, ret)
        {
            if (intersTriangle==triangle)
                continue;
//            LIPEdge abInter(QLineF(intersTriangle.getA(), intersTriangle.getB()));
//            LIPEdge bcInter(QLineF(intersTriangle.getA(), intersTriangle.getB()));
//            LIPEdge acInter(QLineF(intersTriangle.getA(), intersTriangle.getC()));
            //находим треугольники с общими гранями
            if (triangle.isContainEdge(intersTriangle.getA(), intersTriangle.getB())
                    || triangle.isContainEdge(intersTriangle.getB(), intersTriangle.getC())
                    || triangle.isContainEdge(intersTriangle.getA(), intersTriangle.getC()))
            {
                tr_vecs.append(triangle.getCircumcircle());
                tr_vecs.append(intersTriangle.getCircumcircle());
                QPair<LIPCircle, LIPCircle> pair;
                pair.first=triangle.getCircumcircle();
                pair.second=intersTriangle.getCircumcircle();
                pairVec.append(pair);
                QPointF centerTriangle=triangle.getCircumcircle().getCenter();
                QPointF centerInterTriangle=intersTriangle.getCircumcircle().getCenter();
                interTrignales.append(LIPEdge(QLineF(centerTriangle, centerInterTriangle)));
            }
        }

    }
    QPair<QVector<LIPEdge>, QVector<LIPTriangle>> p;
    p.first=interTrignales;
    p.second=ret;

    return p;
}

QVector<LIPCircle> getCircumCircles()
{
    return tr_vecs;
}

QVector<QPair<LIPCircle, LIPCircle> > GetPair()
{
    return pairVec;
}
