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

#include "rtu_server.h"

namespace modbus4qt
{

RtuServer::RtuServer(const QString& portName,
                     QSerialPort::BaudRate baudRate,
                     QSerialPort::DataBits dataBits,
                     QSerialPort::StopBits stopBits,
                     QSerialPort::Parity parity,
                     QObject* parent) :
    Server(parent),
    RTUDevice(portName, baudRate, dataBits, stopBits, parity)
{
    ioDevice_ = serialPort_;

    connect(this, SIGNAL(unitDebugMessage_(QString&)), this, SLOT(onUnitDebugMessage_(QString&)));
    connect(this, SIGNAL(unitErrorMessage_(QString&)), this, SLOT(onUnitErrorMessage_(QString&)));
    connect(this, SIGNAL(unitInfoMessage_(QString&)), this, SLOT(onUnitInfoMessage_(QString&)));

}

//-----------------------------------------------------------------------------

bool RtuServer::configurePort_()
{
    bool result = RTUDevice::configurePort_();

    if (!result)
    {
        emit errorMessage(lastErrorMessage());
    }

    return result;
}

//-----------------------------------------------------------------------------

void RtuServer::onUnitDebugMessage_(const QString& msg)
{
    emit debugMessage(QString("[UnitID: %1] %2").arg(unitID_).arg(msg));
}

//-----------------------------------------------------------------------------

void RtuServer::onUnitErrorMessage_(const QString& msg)
{
    emit errorMessage(QString("[UnitID: %1] %2").arg(unitID_).arg(msg));
}

//-----------------------------------------------------------------------------

void RtuServer::onUnitInfoMessage_(const QString& msg)
{
    emit infoMessage(QString("[UnitID: %1] %2").arg(unitID_).arg(msg));
}


} // namespace modbus4qt

