#ifndef LIPOBJECTTREEMODEL_H
#define LIPOBJECTTREEMODEL_H

#include <QObject>
#include <QAbstractItemModel>
class LIPObjectTreeModel: public QAbstractItemModel
{
    Q_OBJECT
public:
    LIPObjectTreeModel(QObject *parent=nullptr);
    void setColumns(QStringList cols);
    void addItem(QObject *item, const QModelIndex &parentIdx);

protected:
    QStringList columns;
    QObject *rootItem;
    QObject* objByIndex(const QModelIndex &index) const;

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    // QAbstractItemModel interface
public:
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // LIPOBJECTTREEMODEL_H
