#include "TreeItem.h"

#include <QtAlgorithms>

TreeItem::TreeItem(const NodeInfo &data, TreeItem *parentItem)
{
    m_parentItem = parentItem;
    m_itemData = data;
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *child)
{
    m_childItems.append(child);
}

TreeItem *TreeItem::child(int row)
{
    return m_childItems.value(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    return 1;
}

QVariant TreeItem::data(int column) const
{
    switch (column) {
    case 0:
        return QVariant(m_itemData.value);
        break;
    case 1:
        return QVariant(m_itemData.expression);
    }
    return QVariant();
}

int TreeItem::row() const
{
    if(m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}



NodeInfo::NodeInfo()
    :expression(""), value(0)
{}

NodeInfo::NodeInfo(QString expression_, qreal value_)
    :expression(expression_), value(value_)
{}
