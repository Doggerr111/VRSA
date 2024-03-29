#ifndef LIPGRAPHICSITEM_H
#define LIPGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "lipvectorstyle.h"
#include <QGraphicsSceneMouseEvent>
class LIPGraphicsItem :  public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    LIPGraphicsItem();
    ~LIPGraphicsItem();
    /** idk */
    void setPen(QPen pen);
    void setBrush(QBrush brush);
    void setVectorStyle(LIPVectorStyle*);
    void setScaleFactor(double f);
    void select();
    void deselect();
    bool isSelected();
    void setIndex(int ind);
    int getIndex();
signals:
    void clicked(int ind);
protected:
    QPen mPen;
    QBrush mBrush;
    double mPointSize;
    double mSceneScale;
    int mIndex; //индекс в векторе lipvectorlayer

    LIPVectorStyle* mStyle;
    bool mIsSelected;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // LIPGRAPHICSITEM_H
