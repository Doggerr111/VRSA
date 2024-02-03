#ifndef LIPADDINGITEMS_H
#define LIPADDINGITEMS_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVector>

class LIPAddingItems : public QObject
{
    Q_OBJECT
public:
     LIPAddingItems( QGraphicsScene *gScene, QVector<QGraphicsItem*> vec, QObject *parent = nullptr);
public slots:
     void start();
signals:

private:
    QVector<QGraphicsItem*> items;
    QGraphicsScene *scene;

};

#endif // LIPADDINGITEMS_H
