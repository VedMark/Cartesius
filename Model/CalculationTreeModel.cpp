#include "CalculationTreeModel.h"
#include "Calculator/Parser.h"

#include <QStack>

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
    return 2;
}

QVariant CalculationTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
        return static_cast<TreeItem*>(index.internalPointer())->data(index.column());

    return QVariant();
}

Qt::ItemFlags CalculationTreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;

    return Qt::ItemIsEnabled;
}

void CalculationTreeModel::clear()
{
    beginResetModel();
    delete m_rootItem;
    m_rootItem = new TreeItem(NodeInfo());
    endResetModel();
}

void CalculationTreeModel::setupModelData(QString data)
{
    clear();

    beginInsertRows(index(0,0),0, 100);
    findNodes(m_rootItem, data);
    endInsertRows();
}

void CalculationTreeModel::findNodes(TreeItem *parent, QString data)
{
    Parser::Priority lowestPriority = Parser::EXTRA;
    int pos = -1;
    if(data[0] == "(" && data[data.length() - 1] == ")"){
        data = data.mid(1);
        data = data.mid(0, data.length() - 1);
    }

    for(int i = 0; i < data.length(); ++i){
        if(data[i] == "("){
            while(data[++i] != ")");
        }
        if(QRegExp("[a-z]").exactMatch(QString(data[i]))){
            if(lowestPriority >= Parser::HIGH){
                lowestPriority = Parser::HIGH;
                pos = i;
            }
            while(QRegExp("[a-z]").exactMatch(QString(data[++i])));
        }
        if(Parser::isOperator(QString(data[i]))){
            auto priority = Parser::getPriority(QString(data[i]));
            if(lowestPriority >= priority){
                lowestPriority = priority;
                pos = i;
            }
        }
    }

    if(pos == -1)
        parent->appendChild(new TreeItem(NodeInfo("", data.toDouble()), parent));
    else if(QRegExp("[a-z]").exactMatch(QString(data[pos]))){
        parent->appendChild(new TreeItem(NodeInfo(data, Parser::calcucateExpression(data)), parent));
        auto numPos = pos;
        while(data[numPos++] != "(");
        auto subdata = data.mid(numPos, data.length() - numPos - 1);
        findNodes(parent->child(parent->childCount() - 1), subdata);
    }
    else{
        parent->appendChild(new TreeItem(NodeInfo(data, Parser::calcucateExpression(data)), parent));

        if(data[pos] != "√"){
            auto subdata = data.mid(0, pos);
            findNodes(parent->child(parent->childCount() - 1), subdata);
        }
        auto subdata = data.mid(pos + 1, data.length() - 1 - pos);
        findNodes(parent->child(parent->childCount() - 1), subdata);
    }
}
