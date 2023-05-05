#include "descrete_inputs_table_model.h"
#include <QDebug>



void
DescreteInputsTableModel::addCoil(quint16 address)
{
    if (!serverData_->descreteInputs()->contains(address))
    {
        const auto keys = serverData_->descreteInputs()->keys();

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
        serverData_->addDescreteInputs(address);
        endInsertRows();
    }
    else
    {
        // warn address skipped
        qDebug() << "skipped " << address;
    }
}



QVariant
DescreteInputsTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        qDebug() << "idx is invalid";
        return QVariant();
    }

    if (index.row() >= serverData_->descreteInputs()->size())
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
                        return (serverData_->descreteInputs()->begin() + index.row()).key();
                    break;

                    case 2 :
                        return (serverData_->descreteInputs()->begin() + index.row()).value();
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


bool
DescreteInputsTableModel::removeRows(int position, int rows, const QModelIndex& /*parent*/)
{
    return true;
}


int
DescreteInputsTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return serverData_->descreteInputs()->count();
}

//------------------------------------------------------------------------------

bool
DescreteInputsTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{

}

//------------------------------------------------------------------------------

void
DescreteInputsTableModel::beforeCoilsAdded(quint16)
{
}

//------------------------------------------------------------------------------

void
DescreteInputsTableModel::onCoilsAdded(quint16)
{
    emit dataChanged(QModelIndex(), QModelIndex());
//    beginResetModel();
//    endResetModel();
}

//------------------------------------------------------------------------------
// EOF
