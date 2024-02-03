#ifndef LIPTREEWIDGETITEM_H
#define LIPTREEWIDGETITEM_H

#include <QTreeWidgetItem>

class LIPTreeWidgetItem : public QTreeWidgetItem
{
public:
    LIPTreeWidgetItem();
    void setFileName(QString path);

private:
    QString fileName;
};

#endif // LIPTREEWIDGETITEM_H
