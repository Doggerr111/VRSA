#include "liplayertreemodel.h"


LIPLayerTreeModel::LIPLayerTreeModel(QObject *parent)
  : QAbstractItemModel(parent)
{
  //Creating instance of virtual root item
  mRootItem = new QObject(this);
}

void LIPLayerTreeModel::setColumns(QStringList cols)
{
  mColumns = cols;
}

void LIPLayerTreeModel::addItem(QObject *item, const QModelIndex &parentIdx)
{
  beginInsertRows(parentIdx, rowCount(parentIdx), rowCount(parentIdx));
  item->setParent(mObjByIndex(parentIdx));
  endInsertRows();
}

QObject *LIPLayerTreeModel::mObjByIndex(const QModelIndex &index) const
{
  if (!index.isValid())
    return mRootItem;
  return static_cast<QObject*>(index.internalPointer());
}

QModelIndex LIPLayerTreeModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();
  QObject* parentObj = mObjByIndex(parent);
  return createIndex(row, column, parentObj->children().at(row));
}

QModelIndex LIPLayerTreeModel::parent(const QModelIndex &child) const
{
  QObject* childObj = mObjByIndex(child);
  QObject* parentObj = childObj->parent();
  //Virtual root item has invalid index (QModelIndex())
  if (parentObj == mRootItem)
    return QModelIndex();
  //Grand parent item is used to get row coordinate for parent item
  QObject* grandParentObj = parentObj->parent();
  int row = grandParentObj->children().indexOf(parentObj);
  //First row cell is used as parent index
  return createIndex(row, 0, parentObj);
}

int LIPLayerTreeModel::rowCount(const QModelIndex &parent) const
{
  return mObjByIndex(parent)->children().count();
}

int LIPLayerTreeModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return mColumns.count();
}

QVariant LIPLayerTreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();
  //Cell values retrieved from QObject properties
  if (role == Qt::DisplayRole) {
    return mObjByIndex(index)->property(mColumns.at(index.column()).toUtf8());
  }
  return QVariant();
}
