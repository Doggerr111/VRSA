#include "lipobjecttreemodel.h"

LIPObjectTreeModel::LIPObjectTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new QObject(this);
}

void LIPObjectTreeModel::setColumns(QStringList cols)
{
    columns=cols;
}

void LIPObjectTreeModel::addItem(QObject *item, const QModelIndex &parentIdx)
{
    beginInsertRows(parentIdx, rowCount(parentIdx), rowCount(parentIdx));
    item->setParent(objByIndex(parentIdx));
    endInsertRows();
}

QObject *LIPObjectTreeModel::objByIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return rootItem;
    return static_cast<QObject*>(index.internalPointer());
}

QModelIndex LIPObjectTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row,column,parent))
    {
        return QModelIndex();
    }
    QObject *parentObj = objByIndex(parent);
    return createIndex(row, column, parentObj->children().at(row));
}

QModelIndex LIPObjectTreeModel::parent(const QModelIndex &child) const
{
    QObject* childObj = objByIndex(child);
    QObject* parentObj = childObj->parent();
    if (parentObj == rootItem)
        return QModelIndex();
    QObject* grandParentObj = parentObj->parent();
    int row = grandParentObj->children().indexOf(parentObj);
    return createIndex(row, 0, parentObj);
}

int LIPObjectTreeModel::rowCount(const QModelIndex &parent) const
{
    return objByIndex(parent)->children().count();
}

int LIPObjectTreeModel::columnCount(const QModelIndex &parent) const
{
    return columns.count();
}

QVariant LIPObjectTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role == Qt::DisplayRole)
    {
        return objByIndex(index)->property(columns.at(index.column()).toUtf8());
    }


}

Qt::ItemFlags LIPObjectTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return 0;

        Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

        if ( index.column() == 0 )
            flags |= Qt::ItemIsUserCheckable;

        return flags;
}
