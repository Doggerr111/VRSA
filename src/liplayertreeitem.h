#ifndef LIPLAYERTREEITEM_H
#define LIPLAYERTREEITEM_H
#include <QList>
#include <QVariant>

class LIPLayerTreeItem
{
public:
    explicit LIPLayerTreeItem(const QList<QVariant> &data, LIPLayerTreeItem *parentItem = nullptr);
    ~LIPLayerTreeItem();

    void appendChild(LIPLayerTreeItem *child);

    LIPLayerTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    LIPLayerTreeItem *parentItem();

private:
    QList<LIPLayerTreeItem *> mChildItems;
    QList<QVariant> mItemData;
    LIPLayerTreeItem *mParentItem;
};


#endif // LIPLAYERTREEITEM_H
