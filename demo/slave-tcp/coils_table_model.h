#ifndef COILSTABLEMODEL_H
#define COILSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMap>

#include "modbus4qt/server_internal_data.h"

class CoilsTableModel : public QAbstractTableModel
{
        Q_OBJECT
    public:
        explicit CoilsTableModel(modbus4qt::ServerInternalData* serverInternalData, QObject *parent = nullptr);

        void addCoil(quint16 address);

        bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

        int columnCount(const QModelIndex&) const override;

        QVariant data(const QModelIndex& index, int role) const override;

        QVariant headerData(int section, Qt::Orientation, int role) const override;

        Qt::ItemFlags flags(const QModelIndex& index) const override;

        bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

        int rowCount(const QModelIndex& parent) const override;

        bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    public slots:

        void beforeCoilsAdded(quint16);

        void onCoilsAdded(quint16);

    protected:

        modbus4qt::ServerInternalData* serverData_;

};

#endif // COILSTABLEMODEL_H
