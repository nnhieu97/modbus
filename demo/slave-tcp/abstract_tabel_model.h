#pragma once
#include <QAbstractTableModel>
#include <QMap>

#include "modbus4qt/server_internal_data.h"

class ITableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    virtual explicit ITableModel(modbus4qt::ServerInternalData* serverInternalData, QObject *parent = nullptr);

    virtual void addCoil(quint16 address) = 0;

    virtual void addRegisters (quint16 address) = 0;

    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

    int columnCount(const QModelIndex&) const override;

    virtual QVariant data(const QModelIndex& index, int role) const = 0;

    QVariant headerData(int section, Qt::Orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

    int rowCount(const QModelIndex& parent) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

public slots:

    void beforeCellAdded(quint16);

    void onCellAdded(quint16);

protected:

    modbus4qt::ServerInternalData* serverData_;

};
