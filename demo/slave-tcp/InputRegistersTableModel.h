#pragma once
#include <QAbstractTableModel>
#include <QMap>
#include <QDebug>
#include "modbus4qt/server_internal_data.h"


class InputRegistersTableModel : public QAbstractTableModel
{
        Q_OBJECT

    public:
        explicit InputRegistersTableModel(modbus4qt::ServerInternalData* serverInternalData, QObject *parent);
        void addInputRegister(quint16 address);
        bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
        int columnCount(const QModelIndex&) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        QVariant headerData(int section, Qt::Orientation, int role) const override;
        bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
        int rowCount(const QModelIndex& parent) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    public slots:
        void beforeInputRegistersAdded(quint16);
        void onInputRegistersAdded(quint16);
    private:
        modbus4qt::ServerInternalData* serverData_;

};
