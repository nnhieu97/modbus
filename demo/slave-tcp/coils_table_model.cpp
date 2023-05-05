#include "coils_table_model.h"
#include <QDebug>



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



//------------------------------------------------------------------------------
// EOF
