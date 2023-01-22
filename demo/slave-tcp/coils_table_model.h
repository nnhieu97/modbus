#ifndef COILSTABLEMODEL_H
#define COILSTABLEMODEL_H

#include <QAbstractTableModel>

class CoilsTableModel : public QAbstractTableModel
{
        Q_OBJECT
    public:
        explicit CoilsTableModel(QObject *parent = nullptr);
};

#endif // COILSTABLEMODEL_H
