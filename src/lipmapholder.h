#ifndef LIPMAPHOLDER_H
#define LIPMAPHOLDER_H

#include <QGraphicsView>
#include <QObject>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include "lipmapcalculations.h"
/**
 * \class LIPMapHolder
 * \brief Данный класс служит для отображения сцены-карты
 */
class LIPMapHolder : public QGraphicsView
{
    Q_OBJECT
public:
    LIPMapHolder(QObject *parent);
    void zoomToRect(QRectF bRect);
    int getScale();
    QRectF getExtent();
public slots:
    void onAddingFeatures();
    void onStopAddingFeatures();
    void updateAddingFeaturesFlag(bool flag);

private:
    QPointF clickPos;
    bool isDraging;
    bool isAddingFeatures;
    double scaleFactor;
signals:
    void MapHolderResized();
    void MapHolderZoomed(double);
    void extentChanged();

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event);


    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // LIPMAPHOLDER_H
