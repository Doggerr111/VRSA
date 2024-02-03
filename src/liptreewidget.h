#ifndef LIPTREEWIDGET_H
#define LIPTREEWIDGET_H

#include <QTreeWidget>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QObject>

class LIPTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    LIPTreeWidget(QWidget* parent);
signals:
    void itemDropped();
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // LIPTREEWIDGET_H
