#include "coils_table_model.h"

//------------------------------------------------------------------------------

CoilsTableModel::CoilsTableModel(QMap<quint16, bool>* coilsData, QObject *parent)
    : QAbstractTableModel{parent},
      coilsData_(coilsData)
{
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
        return QVariant();
    }

    if (index.row() >= coilsData_->size())
    {
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
//            int i = 0;

//            for (QMap<quint16, bool>::const_iterator iMap = coilsData_->constBegin(); iMap != coilsData_->constEnd(); ++iMap)
//            {
//                if (i != index.row())
//                {
//                    ++i;
//                    continue;
//                }

                switch (index.column())
                {
                    case 0 :
                        return index.row();
                    break;

                    case 1 :
                        return (coilsData_->begin() + index.row()).key();
                    break;

                    case 2 :
                        return (coilsData_->begin() + index.row()).value();
                    break;
                }
//                ++i;
//            }
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
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
    {
//        stringList.removeAt(position);
    }

    endRemoveRows();

    return true;
}

//------------------------------------------------------------------------------

bool
CoilsTableModel::insertRows(int position, int rows, const QModelIndex& /*parent*/)
{
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int i = position; i < position + rows; ++i)
    {
        coilsData_->insert(i + position - 1, false);
    }

    endInsertRows();

    return true;
}

//------------------------------------------------------------------------------

int
CoilsTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return coilsData_->count();
}

//------------------------------------------------------------------------------

bool
CoilsTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{

}

//------------------------------------------------------------------------------
// EOF
