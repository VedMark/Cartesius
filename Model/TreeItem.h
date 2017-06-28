#pragma once

#include <QtGlobal>
#include <QVariant>


struct NodeInfo{
    QString expression;
    qreal value;

    NodeInfo();
    NodeInfo(QString expression, qreal value);
};


class TreeItem
{
public:
    explicit TreeItem(const NodeInfo &data, TreeItem *parentItem = Q_NULLPTR);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();

private:
    QList<TreeItem*> m_childItems;
    NodeInfo m_itemData;
    TreeItem *m_parentItem;
};
