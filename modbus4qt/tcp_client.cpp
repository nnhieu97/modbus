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
    , connectTimeOut_(CONNECTION_TIMEOUT)
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

bool
TcpClient::connectToServer(int timeout)
{
    tcpSocket_->connectToHost(serverAddress_, port_);

    if (!tcpSocket_->waitForConnected(timeout))
    {
        emit errorMessage(tcpSocket_->errorString());
        return false;
    }

    lastTransactionID_ = 0;

    return true;
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
    {
        lastTransactionID_ = 0;
    }
    else
    {
        ++lastTransactionID_;
    }

    return lastTransactionID_;
}

//-----------------------------------------------------------------------------

QHostAddress
TcpClient::getServerAddress() const
{
    return serverAddress_;
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

    getNewTransactionID_();

    result.append((char*)&lastTransactionID_, 2);

    result.append('\x0');
    result.append('\x0');

    result[4] = ((pduSize+1) >> 8) & 0xFF;
    result[5] = (pduSize+1) & 0xFF;

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
    qDebug() << "ADU: " << buf.toHex();
    qDebug() << "ADU size: " << buf.size();

    QByteArray tempBuf(buf);

    // For any case we should check size of recieved data to protect memory
    //
    // На всякий случай проверяем размер полученных данных, чтобы не ничего не испортить
    //
    if (tempBuf.size() > int(sizeof(TcpDataHeader)+sizeof(TCPApplicationDataUnit)))
        tempBuf.resize(sizeof(TcpDataHeader)+sizeof(TCPApplicationDataUnit));

    int tempBufSize = tempBuf.size();

    // Minimum ADU size can be 9 bytes: 2 byte for Transaction ID, 2 bytes for Protocol ID,
    // 2 byte for Length, 1 byte for address, 2 byte for minimum PDU
    //
    // Минимальный размер ADU может быть 9 байт: 2 байт транзакция ИД, 2 байт Протокол ИД,
    // 2 байт длина, 1 байт адрес и 2 байта PDU
    //
    if (tempBufSize < 9)
    {
        emit errorMessage(unitID_, tr("Wrong application data unit recieved!"));
        tempBuf.resize(9);
        tempBufSize = 9;
    }

    TcpDataHeader header;

    WordRec headerTID;
    headerTID.bytes[0] = tempBuf[0];
    headerTID.bytes[1] = tempBuf[1];

    header.transactionId = headerTID.word;

    WordRec headerPID;
    headerPID.bytes[0] = tempBuf[2];
    headerPID.bytes[1] = tempBuf[3];

    header.protocolId = headerPID.word;

    WordRec headerLength;
    headerLength.bytes[0] = tempBuf[4];
    headerLength.bytes[1] = tempBuf[5];

    header.recLength = headerLength.word;

    header.unitId = tempBuf[6];

    TCPApplicationDataUnit adu;

    adu.unitId = tempBuf[6];
    adu.pdu.functionCode = tempBuf[7];

    for (int i = 8; i < tempBufSize; ++i)
        adu.pdu.data[i - 8] = tempBuf[i];

#ifndef QT_NO_DEBUG
    // If we want to print PDU into log file we should remove first 7 byte
    // After that we have pdu in tempBuf
    //
    // Чтобы вывести в лог PDU, удаляем первые 7 байтов
    // Теперь в tempBuf остался только pdu
    //
    tempBuf.remove(0, 7);
    qDebug() << "PDU: " << tempBuf.toHex();
    qDebug() << "PDU size: " << tempBuf.size();
#endif

    return adu.pdu;
}

//-----------------------------------------------------------------------------

QByteArray
TcpClient::readResponse_()
{
    QByteArray inArray;
    inArray.append(ioDevice_->readAll());

    while (ioDevice_->bytesAvailable() || ioDevice_->waitForReadyRead(connectTimeOut_))
    {
        inArray.append(ioDevice_->readAll());
    }

    return inArray;
}

//-----------------------------------------------------------------------------

bool
TcpClient::sendRequestToServer_(const ProtocolDataUnit& requestPDU, int requestPDUSize, ProtocolDataUnit* responsePDU)
{
    if (autoConnect_ && !isConnected())
    {
        if (!connectToServer())
        {
            emit infoMessage(tr("Cannot connect to server!"));

            return false;
        }
    }

    bool result = Client::sendRequestToServer_(requestPDU, requestPDUSize, responsePDU);
    return  result;
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
        if (isConnected())
        {
            disconnectFromServer();
        }

        serverAddress_ = serverAddress;
    }
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt

// EOF


