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

#include <QIODevice>

#include "device.h"
#include "server_internal_data.h"
#include "server.h"


namespace modbus4qt
{

//-----------------------------------------------------------------------------

Server::Server(QObject* parent) :
    QObject(parent)
{
    connect(ioDevice_, SIGNAL(readyRead()), this, SLOT(readClientRequest_()));
}

//-----------------------------------------------------------------------------

Server::Server(ServerInternalData* internalData, QObject* parent) :
    QObject(parent),
    internalData_(internalData)
{
    internalData_->setParent(this);

    connect(ioDevice_, SIGNAL(readyRead()), this, SLOT(readClientRequest_()));
}

//-----------------------------------------------------------------------------

int
Server::readTimeout() const
{
    return readTimeout_;
}

//-----------------------------------------------------------------------------

void
Server::setReadTimeout(int readTimeout)
{
    readTimeout_ = readTimeout;
}

//-----------------------------------------------------------------------------

int
Server::writeTimeout() const
{
    return writeTimeout_;
}

//-----------------------------------------------------------------------------

void
Server::setWriteTimeout(int writeTimeout)
{
    writeTimeout_ = writeTimeout;
}

//-----------------------------------------------------------------------------

bool
Server::modbusServerTransaction(const Device::ProtocolDataUnit& requestPDU, int pduSize)
{
    // Validate function code. If not valid (ExceptionCode = 1;) Send modbus exception response.

    // Validate data address. If not valid (ExceptionCode = 2;) Send modbus exception response.

    // Validate data value.  If not valid (ExceptionCode = 3;) Send modbus exception response.

    // Execute MB function.  If not valid (ExceptionCode = 4, 5, 6;) Send modbus exception response.

    // All's goos. Do some work & send modbus response
    //
    QByteArray response;
    sendDataToClient_(response);

    return true;
}

//-----------------------------------------------------------------------------

bool
Server::sendDataToClient_(const QByteArray& response)
{
    int64_t bytesWritten = ioDevice_->write(response);

    if (bytesWritten <= 0)
    {
        emit errorMessage(tr("Failed to write data, error: %1").arg(ioDevice_->errorString()));
        return false;
    }
    else if (bytesWritten < response.size())
    {
        emit errorMessage(tr("Failed to write all data, error: %1").arg(ioDevice_->errorString()));
        return false;
    }
    else if (!ioDevice_->waitForBytesWritten(writeTimeout_))
    {
        emit errorMessage(tr("Write timeout, error: %1").arg(ioDevice_->errorString()));
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

void
Server::readClientRequest_()
{
    QByteArray inArray;

    //! \todo Think about silence time! See #37
    //
    inArray.append(ioDevice_->readAll());

    while (ioDevice_->bytesAvailable() || ioDevice_->waitForReadyRead(readTimeout_))
    {
        inArray.append(ioDevice_->readAll());
    }

    if (checkReciever_(inArray))
    {
        // We are reciever in incoming packet
        // Do some work

        Device::ProtocolDataUnit requestPdu;
        int pduSize = 0;

        if (processClientRequest_(inArray, requestPdu, pduSize))
        {
            modbusServerTransaction(requestPdu, pduSize);
        }
    }
}

//-----------------------------------------------------------------------------

void
Server::setInternalData(ServerInternalData* internalData)
{
    internalData_ = internalData;
    internalData_->setParent(this);
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt

//-----------------------------------------------------------------------------
// EOF
