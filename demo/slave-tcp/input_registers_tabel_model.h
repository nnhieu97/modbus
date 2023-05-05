#pragma once
#include "abstract_tabel_model.h"
#include <QAbstractTableModel>
#include <QMap>


class InputRegistersTabelModel : ITableModel
{
    Q_OBJECT
public:

    InputRegistersTabelModel(modbus4qt::ServerInternalData* serverInternalData) : ITableModel(serverInternalData){}

    void addRegisters(quint16 address);

    QVariant data(const QModelIndex& index, int role) const override;

    int rowCount(const QModelIndex& parent) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
};

