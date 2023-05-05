#include "input_registers_tabel_model.h"
#include <QDebug>

void
InputRegistersTabelModel::addRegisters(quint16 address)
{
    if (!serverData_->inputRegisters()->contains(address))
    {
        const auto keys = serverData_->inputRegisters()->keys();

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
        serverData_->addInputRegisters(address);
        endInsertRows();
    }
    else
    {
        // warn address skipped
        qDebug() << "skipped " << address;
    }
}



QVariant
InputRegistersTabelModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        qDebug() << "idx is invalid";
        return QVariant();
    }

    if (index.row() >= serverData_->inputRegisters()->size())
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
                        return (serverData_->inputRegisters()->begin() + index.row()).key();
                    break;

                    case 2 :
                        return (serverData_->inputRegisters()->begin() + index.row()).value();
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



int
InputRegistersTabelModel::rowCount(const QModelIndex& /*parent*/) const
{
    return serverData_->inputRegisters()->count();
}

//------------------------------------------------------------------------------

bool
InputRegistersTabelModel::setData(const QModelIndex& index, const QVariant& value, int role)
{

}

//------------------------------------------------------------------------------

void
InputRegistersTabelModel::beforeCoilsAdded(quint16)
{
}

//------------------------------------------------------------------------------

void
InputRegistersTabelModel::onCoilsAdded(quint16)
{
    emit dataChanged(QModelIndex(), QModelIndex());
//    beginResetModel();
//    endResetModel();
}

//------------------------------------------------------------------------------
// EOF


