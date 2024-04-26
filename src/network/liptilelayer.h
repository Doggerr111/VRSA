#ifndef LIPTILELAYER_H
#define LIPTILELAYER_H
#include <liptile.h>

class LIPTileLayer
{
public:
    LIPTileLayer(QString connectionName, QString name);
    ~LIPTileLayer();
    QString getConnectionName();
    QString getName();
    //void setConnectionName(QString name);
    void addTile(LIPTile* newTile);
    void removeTile(LIPTile* tileToRemove);

private:
    QVector<LIPTile*> mTiles;
    QString mConnectionName;
    QString mName;
};

#endif // LIPTILELAYER_H
