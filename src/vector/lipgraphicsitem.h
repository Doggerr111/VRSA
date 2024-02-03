#ifndef LIPGRAPHICSITEM_H
#define LIPGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include "lipvectorstyle.h"
class LIPGraphicsItem : public QGraphicsItem
{

public:
    LIPGraphicsItem();
    ~LIPGraphicsItem();
    void setPen(QPen pen);
    void setBrush(QBrush brush);
    void setVectorStyle(LIPVectorStyle*);
    void setScaleFactor(double f);
    void select();
protected:
    QPen mPen;
    QBrush mBrush;
    double mPointSize;
    double mSceneScale;
    LIPVectorStyle* mStyle;
};

#endif // LIPGRAPHICSITEM_H
