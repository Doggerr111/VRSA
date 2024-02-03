#include "lipcrscombobox.h"
#include <QDebug>
#include <geos/triangulate/DelaunayTriangulationBuilder.h>

LIPCRSComboBox::LIPCRSComboBox(QWidget* parent)
    : QComboBox(parent)
{

    connect(&LIPProject::getInstance(),SIGNAL(newCRS()),this,SLOT(updateCRS()));
    //QObject::connect(LIPProject::getInstance(), &LIPProject::newCRS, this, &LIPCRSComboBox::updateCRS);

    //connect(LIPProject::getInstance(), &LIPProject::newCRS, this, &LIPCRSComboBox::updateCRS);
    int index=0;
    foreach(LIPCoordinateSystem *crs, LIPProject::getInstance().getCoordinateSystems())
    {
        addItem(crs->getName());
        //setItemData(index, layer->returnFileName());
        index++;
    }
        //using namespace geos::geom;
//    geos::algorithm::Area ar;
    geos::geom::Coordinate cr;
     geos::geom::LineSegment crfa;
     crfa.angle();
    cr.setNull();
    qDebug()<<cr.isNull();
//    cr.setNull();
}

void LIPCRSComboBox::updateCRS()
{
    clear();
    foreach(LIPCoordinateSystem *crs, LIPProject::getInstance().getCoordinateSystems())
    {
        addItem(crs->getName());
        //setItemData(index, layer->returnFileName());
        //index++;
    }
}

