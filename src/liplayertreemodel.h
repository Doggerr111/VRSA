#ifndef LIPLAYERTREEMODEL_H
#define LIPLAYERTREEMODEL_H
#include <QAbstractItemModel>
#include "liplayertreeitem.h"


class LIPLayerTreeModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  LIPLayerTreeModel(QObject* parent = nullptr);
  //Registers columns
  void setColumns(QStringList cols);
  //Adds child item from given parent item (parentIdx)
  void addItem(QObject* item, const QModelIndex &parentIdx);

protected:
  QStringList mColumns;
  //Virtual root item used to store the list of root content items
  QObject* mRootItem;
  //Convenient method to retrieve item object from index
  QObject* mObjByIndex(const QModelIndex &index) const;

  // QAbstractItemModel interface
public:
  //Abstract virtual methods that needs to be implemented
  virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
  virtual QModelIndex parent(const QModelIndex &child) const override;
  virtual int rowCount(const QModelIndex &parent) const override;
  virtual int columnCount(const QModelIndex &parent) const override;
  virtual QVariant data(const QModelIndex &index, int role) const override;
};

#endif // LIPLAYERTREEMODEL_H
