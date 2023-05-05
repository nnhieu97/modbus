#pragma once
#include <QAbstractTableModel>
#include <QMap>

#include "modbus4qt/server_internal_data.h"

class ITableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ITableModel(modbus4qt::ServerInternalData* serverInternalData, QObject *parent = nullptr);

    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

    int columnCount(const QModelIndex&) const override;

    QVariant headerData(int section, Qt::Orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

public slots:

    void beforeCellsAdded(quint16);

    void onCellsAdded(quint16);

protected:

    modbus4qt::ServerInternalData* serverData_;

};
