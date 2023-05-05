#pragma once
#include <QAbstractTableModel>
#include <QMap>

#include "abstract_tabel_model.h"

class CoilsTableModel : public ITableModel
{
        Q_OBJECT
    public:

    CoilsTableModel(modbus4qt::ServerInternalData* serverInternalData) : ITableModel(serverInternalData){}

    void addCoil(quint16 address);

    QVariant data(const QModelIndex& index, int role) const override;

    int rowCount(const QModelIndex& parent) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role) override;


};

