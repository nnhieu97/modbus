#include "InputRegistersTableModel.h"


InputRegistersTableModel::InputRegistersTableModel(modbus4qt::ServerInternalData* serverInternalData, QObject *parent = nullptr):
    QAbstractTableModel{parent},
    serverData_{serverInternalData}
{
     connect(serverData_, SIGNAL(InputRegistersAdded(quint16)), this, SLOT(onInputRegistersAdded(quint16)));
}

void
InputRegistersTableModel::addInputRegister(quint16 address)
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
        serverData_->addInputRegister(address);
        endInsertRows();
    }
    else
    {
        // warn address skipped
        qDebug() << "skipped " << address;
    }
}

bool
InputRegistersTableModel::insertRows(int position, int rows, const QModelIndex& /*parent*/)
{
//    beginInsertRows(QModelIndex(), position, position + rows - 1);

//    for (int i = position; i < position + rows; ++i)
//    {
//        //serverData_->addCoil();
//    }

//    endInsertRows();

    return true;
}

int InputRegistersTableModel::columnCount(const QModelIndex&) const
{

}

QVariant InputRegistersTableModel::data(const QModelIndex& index, int role) const
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

QVariant InputRegistersTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex())
{
    //    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    //    for (int row = 0; row < rows; ++row)
    //    {
    ////        stringList.removeAt(position);
    //    }

    //    endRemoveRows();

        return true;
}

bool InputRegistersTableModel::removeRows(int position, int rows, const QModelIndex& /*parent = QModelIndex()*/)
{
    //    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    //    for (int row = 0; row < rows; ++row)
    //    {
    ////        stringList.removeAt(position);
    //    }

    //    endRemoveRows();

        return true;
}

int InputRegistersTableModel::rowCount(const QModelIndex& parent) const
{
    return serverData_->inputRegisters()->count();
}

bool InputRegistersTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{

}

void beforeInputRegistersAdded(quint16)
{

}

void InputRegistersTableModel::onInputRegistersAdded(quint16)
{
    emit dataChanged(QModelIndex(), QModelIndex());
//    beginResetModel();
//    endResetModel();
}


