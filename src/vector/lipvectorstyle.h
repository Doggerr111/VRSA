#ifndef LIPVECTORSTYLE_H
#define LIPVECTORSTYLE_H
#include <QPainter>
#include <QPen>
#include <QBrush>
#include "liptypes.h"
#include <QApplication>
#include <QScreen>
#include <QRandomGenerator>
class LIPVectorStyle
{
public:
    LIPVectorStyle();
    ~LIPVectorStyle();
    void setStyleUnit(LIPStyleUnit units);
    void setPen(QPen pen);
    void setWidthMM(double width);
    void setWidth(double width);
    void setPenColor(QColor penCol);
    void setPointSize(double sizeF);
    void setBrush(QBrush brush);
    void setBrushColor(QColor brCol);
    void setGeomType(LIPGeometryType type);
    QPen getPen();
    QBrush getBrush();
    double getPointSize();
    LIPGeometryType GetGeomType();
public:
    static double pixelToMM(double pix);
    static double MMToPixel(double mm);
    /** Формирует стандарnный стиль для векторного слоя в
     *  зависсимости от типа геометрии*/
    static LIPVectorStyle* createDefaultVectorStyle(LIPGeometryType type);
    LIPVectorStyle selectedStyle(LIPVectorStyle* style);
private:

    LIPStyleUnit mStyleUnit;
    LIPGeometryType mGeomType;
    QPen mPen;
    QBrush mBrush;
    double mSceneScaleFact;
    double mPointSize;

};

#endif // LIPVECTORSTYLE_H
