#include "lipmapholder.h"

LIPMapHolder::LIPMapHolder(QObject *parent)
    : QGraphicsView{},
      isDraging{false}
{

}

void LIPMapHolder::resizeEvent(QResizeEvent *event)
{
    emit MapHolderResized();
    QGraphicsView::resizeEvent(event);

}

void LIPMapHolder::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y()<0 )
    {
        scaleFactor=0.3;

       scale(scaleFactor, scaleFactor);
    }
    else if (event->angleDelta().y()>0) {
        scaleFactor=2;
        scale(scaleFactor, scaleFactor);
    }
    emit MapHolderZoomed(scaleFactor);
}

void LIPMapHolder::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if (event->button()==Qt::LeftButton)
        isDraging=true;
    clickPos=event->pos();
}

void LIPMapHolder::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    isDraging=false;
}

void LIPMapHolder::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    if (isDraging)
    {
        QPointF delta=clickPos-event->pos();
        clickPos=event->pos();
        QPoint rect=viewport()->rect().center();
        this->centerOn(mapToScene(this->viewport()->rect().center()+delta.toPoint()));
        //scene()->setSceneRect(sceneRect().translated(-delta));
//        QMatrix const matrix = this->matrix().inverted();
//        QRectF visibleRect = matrix.mapRect(this->viewport()->rect());
//        visibleRect.moveTopLeft(matrix.map(QPoint(this->horizontalScrollBar()->value(),
//                                                this->verticalScrollBar()->value())));
//        int x=visibleRect.x()+mapToScene(event->pos()).x()-mapToScene(clickPos).x();
//        int y=visibleRect.y()+mapToScene(event->pos()).y()-mapToScene(clickPos).y();
//        QRectF rect(x,y,visibleRect.width(), visibleRect.height());
//        this->fitInView(rect);
        //setSceneRect(rect);
    }
}


