#ifndef LIPEDGE_H
#define LIPEDGE_H

#include <QLineF>

class LIPEdge : public QLineF
{
public:
    LIPEdge(QLineF l);


    bool isBad;
private:

};

#endif // LIPEDGE_H
