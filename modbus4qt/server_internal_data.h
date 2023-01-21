#ifndef MODBUS4QT_SERVERINTERNALDATA_H
#define MODBUS4QT_SERVERINTERNALDATA_H

#include <QObject>

namespace modbus4qt {

class ServerInternalData : public QObject
{
        Q_OBJECT
    public:
        explicit ServerInternalData(QObject *parent = nullptr);

    signals:

};

} // namespace modbus4qt

#endif // MODBUS4QT_SERVERINTERNALDATA_H
