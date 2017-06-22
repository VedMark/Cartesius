#include "CalculationTreeModel.h"

CalculationTreeModel::CalculationTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new TreeItem(NodeInfo());
}

CalculationTreeModel::~CalculationTreeModel()
{
    delete m_rootItem;
}

QVariant CalculationTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return QVariant();
}

QModelIndex CalculationTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;
    if(!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    auto childItem = parentItem->child(row);
    if(childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex CalculationTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();

    auto childItem = static_cast<TreeItem*>(index.internalPointer());
    auto parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int CalculationTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int CalculationTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant CalculationTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
        return static_cast<TreeItem*>(index.internalPointer())->data();

    return QVariant();
}

Qt::ItemFlags CalculationTreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

void CalculationTreeModel::setupModelData(QString data)
{

}
