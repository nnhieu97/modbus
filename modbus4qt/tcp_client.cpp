/*****************************************************************************
 * modbus4qt Library
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 * Copyright (C) 2012-2015
 * http://mt11.net.ru
 *****************************************************************************/

/*****************************************************************************
* This file is part of modbus4qt.
*
* modbus4qt is free software: you can redistribute it and/or modify it under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option) any
* later version.
*
* modbus4qt is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
* A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* modbus4qt.
*
* If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QDateTime>
#include <QTcpSocket>
#include <QDebug>

#include "memory_utils.h"
#include "tcp_client.h"

namespace modbus4qt
{

//-----------------------------------------------------------------------------

TcpClient::TcpClient(QObject *parent) :
    Client(parent),
    autoConnect_(true),
    connectTimeOut_(DEFAULT_TIMEOUT),
    serverAddress_(QHostAddress("127.0.0.1"))
{
    unitID_				= IGNORE_UNIT_ID;

    ioDevice_ = tcpSocket_;
}

//-----------------------------------------------------------------------------

bool
TcpClient::connectToServer(int timeout)
{
    tcpSocket_->connectToHost(serverAddress_, port_);

    if (!tcpSocket_->waitForConnected(timeout))
    {
        emit errorMessage(tcpSocket_->errorString());
        return false;
    }

    transactionID_ = 0;

    return true;
}

//-----------------------------------------------------------------------------

void
TcpClient::disconnectFromServer()
{
    tcpSocket_->disconnectFromHost();
}

//-----------------------------------------------------------------------------

uint16_t
TcpClient::nextTransactionID_()
{
    if (transactionID_ == 0xFFFF)
    {
        transactionID_ = 0;
    }
    else
    {
        ++transactionID_;
    }

    return transactionID_;
}

//-----------------------------------------------------------------------------

QHostAddress
TcpClient::getServerAddress() const
{
    return serverAddress_;
}

//-----------------------------------------------------------------------------

bool
TcpClient::autoConnect() const
{
    return autoConnect_;
}

//-----------------------------------------------------------------------------

bool
TcpClient::isConnected() const
{
    return (tcpSocket_->state() == QAbstractSocket::ConnectedState);
}

//-----------------------------------------------------------------------------

void
TcpClient::onUnitDebugMessage_(const QString& msg)
{
    emit debugMessage(QString("[Server: %1] %2").arg(serverAddress_.toString()).arg(msg));
}

//-----------------------------------------------------------------------------

void
TcpClient::onUnitErrorMessage_(const QString& msg)
{
    emit debugMessage(QString("[Server: %1] %2").arg(serverAddress_.toString()).arg(msg));
}

//-----------------------------------------------------------------------------

void
TcpClient::onUnitInfoMessage_(const QString& msg)
{
    emit debugMessage(QString("[Server: %1] %2").arg(serverAddress_.toString()).arg(msg));
}

//-----------------------------------------------------------------------------

bool
TcpClient::sendDataToServer_(const QByteArray& request)
{
    if (autoConnect_ && !isConnected())
    {
        if (!connectToServer())
        {
            emit infoMessage(tr("Cannot connect to server!"));

            return false;
        }
    }

    bool result = Client::sendDataToServer_(request);

    return result;
}

//-----------------------------------------------------------------------------

bool
TcpClient::sendRequestToServer_(const ProtocolDataUnit& requestPDU, int requestPDUSize)
{
    QByteArray adu = prepareADU_(requestPDU, requestPDUSize);

    return sendDataToServer_(adu);
}

//-----------------------------------------------------------------------------

bool TcpClient::readResponseFromServer_(ProtocolDataUnit& pdu)
{
    QByteArray inArray;

    if (!readDataFromServer_(inArray))
    {
        return false;
    }

    pdu = getPduFromAdu_(inArray);

    return true;
}

//-----------------------------------------------------------------------------

void
TcpClient::setAutoConnect(bool autoConnect)
{
    autoConnect_ = autoConnect;
}

//-----------------------------------------------------------------------------

void
TcpClient::setServerAddress(const QHostAddress& serverAddress, int port)
{
    if (serverAddress != serverAddress_)
    {
        if (isConnected())
        {
            disconnectFromServer();
        }

        serverAddress_ = serverAddress;

        setServerPort(port);
    }
}

//-----------------------------------------------------------------------------

void
TcpClient::setServerPort(const int& port)
{
    if (port != port_)
    {
        if (isConnected())
        {
            disconnectFromServer();
        }

        port_ = port;
    }
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt

// EOF


