#include "liptilelayer.h"
#include "mainwindow.h"
LIPTileLayer::LIPTileLayer(QString connectionName, QString name)
    : mConnectionName{connectionName},
      mName{name}
{
    //LIPWidgetManager::getInstance().getMainWindow()->addTileLayer(this);
}

LIPTileLayer::~LIPTileLayer()
{
    for (LIPTile* tile: mTiles)
        delete tile;
    mTiles.clear();
}

QString LIPTileLayer::getName()
{
    return mName;
}

void LIPTileLayer::addTile(LIPTile *newTile)
{
    mTiles.append(newTile);
}

void LIPTileLayer::removeTile(LIPTile *tileToRemove)
{

}
