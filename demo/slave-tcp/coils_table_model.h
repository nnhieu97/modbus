#ifndef COILSTABLEMODEL_H
#define COILSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMap>

class CoilsTableModel : public QAbstractTableModel
{
        Q_OBJECT
    public:
        explicit CoilsTableModel(QMap<quint16, bool>* coilsData, QObject *parent = nullptr);

        virtual bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

        virtual int columnCount(const QModelIndex&) const override;

        virtual QVariant data(const QModelIndex& index, int role) const override;

        virtual QVariant headerData(int section, Qt::Orientation, int role) const override;

        virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

        virtual bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

        virtual int rowCount(const QModelIndex& parent) const override;

        virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    protected:

        QMap<quint16, bool>* coilsData_;
};

#endif // COILSTABLEMODEL_H
