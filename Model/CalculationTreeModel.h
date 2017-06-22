#pragma once

#include <QAbstractItemModel>

#include "Model/TreeItem.h"

class CalculationTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit CalculationTreeModel(QObject *parent = nullptr);
    ~CalculationTreeModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_SLOT void setupModelData(QString data);

private:
    TreeItem *m_rootItem;
};
