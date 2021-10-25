/*****************************************************************************
 * modbus4qt Library
 * Copyright (C) 2012 MELZ-Invest JSC
 * http://www.melz-invest.ru/projects/modbus4qt
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 *****************************************************************************/

/*

The contents of this file are subject to the Mozilla Public License Version 1.1
(the "License"); you may not use this file except in compliance with the
License. You may obtain a copy of the License at http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
the specific language governing rights and limitations under the License.

Alternatively, the contents of this file may be used under the terms of the
GNU General Public License Version 2 or later (the "GPL"), in which case
the provisions of the GPL are applicable instead of those above. If you wish to
allow use of your version of this file only under the terms of the GPL and not
to allow others to use your version of this file under the MPL, indicate your
decision by deleting the provisions above and replace them with the notice and
other provisions required by the GPL. If you do not delete the provisions
above, a recipient may use your version of this file under either the MPL or
the GPL.

This file based on: IdModbusClient.pas,v 1.31 2011/07/04 13:05:27 plpolak Exp $
from delhimodbus-1.5.0

*/


#include "tcp_client.h"
#include "consts.h"
#include "utils.h"

#include <QDateTime>
#include <QDebug>

namespace modbus4qt
{

//-----------------------------------------------------------------------------

TcpClient::TcpClient(QObject *parent) :
    Client(parent)
    , connectTimeOut_(15000)
    , lastTransactionID_(0)
{
    autoConnect_		= true;
    port_				= DefaultTcpPort;
    serverAddress_		= QHostAddress("127.0.0.1");
    unitID_				= IgnoreUnitId;

    ioDevice_ = new QTcpSocket(this);
    tcpSocket_ = dynamic_cast<QTcpSocket*>(ioDevice_);

//    onResponseError = nullptr;
//    onResponseMismatch = nullptr;
}

//-----------------------------------------------------------------------------

void
TcpClient::connectToServer(int timeout /* = IdTimeoutDefault*/ )
{
    tcpSocket_->connectToHost(serverAddress_, port_);
    if (!tcpSocket_->waitForConnected(timeout))
        emit errorMessage(tcpSocket_->errorString());

    lastTransactionID_ = 0;
}

//-----------------------------------------------------------------------------

void
TcpClient::disconnectFromServer()
{
    tcpSocket_->disconnectFromHost();
}

////-----------------------------------------------------------------------------

//void
//TcpClient::doResponseError_(quint8 functionCode, quint8 errorCode, const ResponseBuffer& responseBuffer)
//{
//}

//-----------------------------------------------------------------------------

//void
//TcpClient::doResponseMismatch_(quint8 requestFunctionCode, quint8 responseFunctionCode, const ResponseBuffer& responseBuffer)
//{
//}

//-----------------------------------------------------------------------------

quint16
TcpClient::getNewTransactionID_()
{
    if (lastTransactionID_ == 0xFFFF)
        lastTransactionID_ = 0;
    else
        ++lastTransactionID_;

    return lastTransactionID_;
}

//-----------------------------------------------------------------------------

bool
TcpClient::isAutoConnect() const
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

QByteArray
TcpClient::prepareADU_(const ProtocolDataUnit& pdu, int pduSize)
{
    QByteArray result;

    result.append(hi(lastTransactionID_));
    result.append(lo(lastTransactionID_));

    result.append('\x0');
    result.append('\x0');

    QString size;
    size = QString::number(pduSize+1,16);
    QByteArray sz = QByteArray::fromHex(size.toUtf8());
    result.append(sz);

    result.append(unitID_);

    result.insert(7, (char*)&pdu, pduSize);

    qDebug() << "ADU: " << result.toHex();
    qDebug() << "ADU size: " << result.size();

    return result;
}

//-----------------------------------------------------------------------------

ProtocolDataUnit
TcpClient::processADU_(const QByteArray& buf)
{
}

//-----------------------------------------------------------------------------

QByteArray
TcpClient::readResponse_()
{
}

//-----------------------------------------------------------------------------

QHostAddress
TcpClient::getServerAddress() const
{
    return serverAddress_;
}

//-----------------------------------------------------------------------------

void
TcpClient::setAutoConnect(bool autoConnect)
{
    autoConnect_ = autoConnect;
}

//-----------------------------------------------------------------------------

void
TcpClient::setServerAddress(const QHostAddress& serverAddress)
{
    if (serverAddress != serverAddress_)
    {
        if (isConnected()) disconnectFromServer();
        serverAddress_ = serverAddress;
    }
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt

// EOF


