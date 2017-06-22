#pragma once

#include <QtGlobal>
#include <QVariant>

enum OperationType{
    SUM,
    SUBSTRACTION,
    MULTIPLICATION,
    DIVISION,
    SQUARE_ROOT,
    PERCENT,
    SIN,
    COS,
    TG,
    CTG,
    ARCSIN,
    ARCCOS,
    ARCTG,
    ARCCTG,
    NONE
};


struct NodeInfo{
    OperationType operation;
    qreal value;
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
    QVariant data() const;
    int row() const;
    TreeItem *parentItem();

private:
    QList<TreeItem*> m_childItems;
    NodeInfo m_itemData;
    TreeItem *m_parentItem;
};
