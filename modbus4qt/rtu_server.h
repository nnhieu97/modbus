#ifndef RTU_SERVER_H
#define RTU_SERVER_H

#include "server.h"

namespace modbus4qt
{

class RtuServer : public Server
{
    Q_OBJECT
    public:
        explicit RtuServer(QObject *parent = 0);

    signals:

    public slots:

};

} // namespace modbus4qt

#endif // RTU_SERVER_H
