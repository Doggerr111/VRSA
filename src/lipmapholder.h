#ifndef LIPMAPHOLDER_H
#define LIPMAPHOLDER_H

#include <QGraphicsView>
#include <QObject>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QScrollBar>
/**
 * \class LIPMapHolder
 * \brief Данный класс служит для отображения сцены-карты
 */
class LIPMapHolder : public QGraphicsView
{
    Q_OBJECT
public:
    LIPMapHolder(QObject *parent);



private:
    QPoint clickPos;
    bool isDraging;
    double scaleFactor;
signals:
    void MapHolderResized();
    void MapHolderZoomed(double);

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
