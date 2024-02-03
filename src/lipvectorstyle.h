#ifndef LIPVECTORSTYLE_H
#define LIPVECTORSTYLE_H
#include <QPainter>
#include <QPen>
#include <QBrush>
#include "liptypes.h"

class LIPVectorStyle
{
public:
    LIPVectorStyle();
    void setStyleUnit(LIPStyleUnit units);
    void setPen(QPen pen);
    void setBrush(QBrush brush);
    QPen getPen();
    QBrush getBrush();
private:

    LIPStyleUnit mStyleUnit;
    QPen mPen;
    QBrush mBrush;




};

#endif // LIPVECTORSTYLE_H
