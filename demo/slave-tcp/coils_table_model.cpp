#include <QDebug>

#include "coils_table_model.h"

//------------------------------------------------------------------------------

CoilsTableModel::CoilsTableModel(modbus4qt::ServerInternalData* serverInternalData, QObject *parent)
    : QAbstractTableModel{parent},
      serverData_{serverInternalData}
{
    connect(serverData_, SIGNAL(coilAdded(quint16)), this, SLOT(onCoilsAdded(quint16)));
}

//------------------------------------------------------------------------------

void
CoilsTableModel::addCoil(quint16 address)
{
    if (!serverData_->coils()->contains(address))
    {
        const auto keys = serverData_->coils()->keys();

        int i = 0;

        for (auto key : keys)
        {
            i++;

            if (address < key)
            {
                continue;
            }
            else
            {
                i--;
                break;
            }
        }

        beginInsertRows(QModelIndex(), i, i);
        serverData_->addCoil(address);
        endInsertRows();
    }
    else
    {
        // warn address skipped
        qDebug() << "skipped " << address;
    }
}

//------------------------------------------------------------------------------

int
CoilsTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 3;
};

//------------------------------------------------------------------------------

QVariant
CoilsTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        qDebug() << "idx is invalid";
        return QVariant();
    }

    if (index.row() >= serverData_->coils()->size())
    {
        qDebug() << "idx > size";
        return QVariant();
    }

    switch (role)
    {
        case Qt::TextAlignmentRole :
        {
            return int(Qt::AlignCenter | Qt::AlignVCenter);
        }
        break;

        case Qt::DisplayRole :
        {
                switch (index.column())
                {
                    case 0 :
                        return index.row();
                    break;

                    case 1 :
                        return (serverData_->coils()->begin() + index.row()).key();
                    break;

                    case 2 :
                        return (serverData_->coils()->begin() + index.row()).value();
                    break;
                }
        }
        break;
        case Qt::EditRole :
        {
            //
        }
        break;
    }

    return QVariant();
}

//------------------------------------------------------------------------------

QVariant
CoilsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
                case 0 :
                    return QVariant(tr("#"));
                break;
                case 1 :
                    return QVariant(tr("Address"));
                break;
                case 2 :
                    return QVariant(tr("Value"));
                break;
            }
        }
    }

    return QVariant();
}

//------------------------------------------------------------------------------

Qt::ItemFlags
CoilsTableModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() > 0)
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

//------------------------------------------------------------------------------

bool
CoilsTableModel::removeRows(int position, int rows, const QModelIndex& /*parent*/)
{
//    beginRemoveRows(QModelIndex(), position, position + rows - 1);

//    for (int row = 0; row < rows; ++row)
//    {
////        stringList.removeAt(position);
//    }

//    endRemoveRows();

    return true;
}

//------------------------------------------------------------------------------

bool
CoilsTableModel::insertRows(int position, int rows, const QModelIndex& /*parent*/)
{
//    beginInsertRows(QModelIndex(), position, position + rows - 1);

//    for (int i = position; i < position + rows; ++i)
//    {
//        //serverData_->addCoil();
//    }

//    endInsertRows();

    return true;
}

//------------------------------------------------------------------------------

int
CoilsTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return serverData_->coils()->count();
}

//------------------------------------------------------------------------------

bool
CoilsTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{

}

//------------------------------------------------------------------------------

void
CoilsTableModel::beforeCoilsAdded(quint16)
{
}

//------------------------------------------------------------------------------

void
CoilsTableModel::onCoilsAdded(quint16)
{
    emit dataChanged(QModelIndex(), QModelIndex());
//    beginResetModel();
//    endResetModel();
}

//------------------------------------------------------------------------------
// EOF
