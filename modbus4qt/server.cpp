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

#include "server.h"

#include <QIODevice>

namespace modbus4qt
{

//-----------------------------------------------------------------------------

Server::Server(QObject *parent)
    : AbstractDevice(parent)
{
}

//-----------------------------------------------------------------------------

void
Server::addCoil(quint16 index)
{
    if (coils_.contains(index))
    {
        return;
    }

    coils_[index] = false;
}

//-----------------------------------------------------------------------------

void
Server::addCoils(quint16 startIndex, quint16 endIndex)
{
    for (quint16 i = startIndex; i <= endIndex; i++)
    {
        addCoil(i);
    }
}

//-----------------------------------------------------------------------------

void
Server::addDescreteInput(quint16 index)
{
    if (descreteInputs_.contains(index))
    {
        return;
    }

    descreteInputs_[index] = false;
}

//-----------------------------------------------------------------------------

void
Server::addDescreteInputs(quint16 startIndex, quint16 endIndex)
{
    for (quint16 i = startIndex; i <= endIndex; i++)
    {
        addDescreteInput(i);
    }
}

//-----------------------------------------------------------------------------

void
Server::addHoldingRegister(quint16 index)
{
    if (holdingRegisters_.contains(index))
    {
        return;
    }

    holdingRegisters_[index] = 0;
}

//-----------------------------------------------------------------------------

void
Server::addHoldingRegisters(quint16 startIndex, quint16 endIndex)
{
    for (quint16 i = startIndex; i <= endIndex; i++)
    {
        addHoldingRegister(i);
    }
}

//-----------------------------------------------------------------------------

void
Server::addInputRegister(quint16 index)
{
    if (inputRegisters_.contains(index))
    {
        return;
    }

    inputRegisters_[index] = 0;
}

//-----------------------------------------------------------------------------

void
Server::addInputRegisters(quint16 startIndex, quint16 endIndex)
{
    for (quint16 i = startIndex; i <= endIndex; i++)
    {
        addInputRegister(i);
    }

}

//-----------------------------------------------------------------------------

void
Server::processIncomingRequest()
{
    QByteArray inArray;

    // Read all and after wait at least silence time for RTU Server
    //
    inArray.append(ioDevice_->readAll());

    //! \todo Think about silence time!
    while (ioDevice_->bytesAvailable()) // || ioDevice_->waitForReadyRead(silenceTime_ * 2))
    {
        inArray.append(ioDevice_->readAll());
    }

    // Validate function code. If not valid ExceptionCode = 1; Send modbus exception response.

    // Validate data address. If not valid ExceptionCode = 2; Send modbus exception response.

    // Validate data value.  If not valid ExceptionCode = 3; Send modbus exception response.

    // Execute MB function.  If not valid ExceptionCode = 4, 5, 6; Send modbus exception response.

    // Send modbus response
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt

//-----------------------------------------------------------------------------
// EOF
