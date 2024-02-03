#include "liptreewidgetitem.h"

LIPTreeWidgetItem::LIPTreeWidgetItem()
{

}

void LIPTreeWidgetItem::setFileName(QString path)
{
    if (!path.isEmpty())
        fileName=path;
}
