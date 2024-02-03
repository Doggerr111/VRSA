#include "lipaddingitems.h"

LIPAddingItems::LIPAddingItems(QGraphicsScene *gScene, QVector<QGraphicsItem*> vec, QObject *parent)
    : QObject{parent},
      items{vec},
      scene{gScene}
{


}

void LIPAddingItems::start()
{
    for(int i=0; i<items.size(); i++)
    {
        QGraphicsEllipseItem* item = new QGraphicsEllipseItem();
        item->setRect(items.at(i)->x(),items.at(i)->y(),0.01,0.01);
        scene->addItem(item);
    }
}

