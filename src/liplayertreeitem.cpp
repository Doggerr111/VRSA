#include "liplayertreeitem.h"

LIPLayerTreeItem::LIPLayerTreeItem(const QList<QVariant> &data, LIPLayerTreeItem *parentItem)
    : mItemData(data), mParentItem(parentItem)
{

}

LIPLayerTreeItem::~LIPLayerTreeItem()
{
    qDeleteAll(mChildItems);
}

void LIPLayerTreeItem::appendChild(LIPLayerTreeItem *item)
{
    mChildItems.append(item);
}

LIPLayerTreeItem *LIPLayerTreeItem::child(int row)
{
    if (row < 0 || row >= mChildItems.size())
        return nullptr;
    return mChildItems.at(row);
}

int LIPLayerTreeItem::childCount() const
{
    return mChildItems.count();
}

int LIPLayerTreeItem::row() const
{
    if (mParentItem)
        return mParentItem->mChildItems.indexOf(const_cast<LIPLayerTreeItem*>(this));

    return 0;
}

int LIPLayerTreeItem::columnCount() const
{
    return mItemData.count();
}
QVariant LIPLayerTreeItem::data(int column) const
{
    if (column < 0 || column >= mItemData.size())
        return QVariant();
    return mItemData.at(column);
}
LIPLayerTreeItem *LIPLayerTreeItem::parentItem()
{
    return mParentItem;
}
