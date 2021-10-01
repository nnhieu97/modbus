#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "server.h"

namespace modbus4qt
{

class TcpServer : public Server
{
    Q_OBJECT
    public:
        TcpServer();
};

}

#endif // TCPSERVER_H
