#include "abstract_tabel_model.h"


ITableModel::ITableModel (modbus4qt::ServerInternalData* serverInternalData, QObject *parent)
                        : QAbstractTableModel{parent},
                        serverData_{serverInternalData}
{
    connect(serverData_, SIGNAL(cellAdded(quint16)), this, SLOT(onCellAdded(quint16)));
}

bool
ITableModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    return true;
}


int ITableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 3;
}

QVariant
ITableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags
ITableModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() > 0)
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

bool ITableModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    return true;
}

void
ITableModel::beforeCellsAdded(quint16)
{
}

//------------------------------------------------------------------------------

void
ITableModel::onCellsAdded(quint16)
{
    emit dataChanged(QModelIndex(), QModelIndex());
//    beginResetModel();
//    endResetModel();
}






