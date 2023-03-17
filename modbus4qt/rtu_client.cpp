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

#include <QDebug>
#include <QMutex>
#include <QWaitCondition>

#include "rtu_client.h"

namespace modbus4qt
{

//-----------------------------------------------------------------------------

RtuClient::RtuClient(const QString& portName,
                   QSerialPort::BaudRate baudRate,
                   QSerialPort::DataBits dataBits,
                   QSerialPort::StopBits stopBits,
                   QSerialPort::Parity parity,
                   QObject *parent)
    : Client(parent),
      RTUDevice(portName, baudRate, dataBits, stopBits, parity),
      inSilenceState_(false),
      silenceTimer_(this)
{
    ioDevice_ = serialPort_;

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    connect(this, SIGNAL(unitDebugMessage_(QString&)), this, SLOT(onUnitDebugMessage_(QString&)));
    connect(this, SIGNAL(unitErrorMessage_(QString&)), this, SLOT(onUnitErrorMessage_(QString&)));
    connect(this, SIGNAL(unitInfoMessage_(QString&)), this, SLOT(onUnitInfoMessage_(QString&)));

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::RtuClient(const QString& portName,
                   QSerialPort::BaudRate baudRate,
                   QSerialPort::DataBits dataBits,
                   QSerialPort::StopBits stopBits,
                   QObject *parent)
    : Client(parent),
      RTUDevice(portName, baudRate, dataBits, stopBits),
      inSilenceState_(false),
      silenceTimer_(this)
{
    ioDevice_ = serialPort_;

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    connect(this, SIGNAL(unitDebugMessage_(QString&)), this, SLOT(onUnitDebugMessage_(QString&)));
    connect(this, SIGNAL(unitErrorMessage_(QString&)), this, SLOT(onUnitErrorMessage_(QString&)));
    connect(this, SIGNAL(unitInfoMessage_(QString&)), this, SLOT(onUnitInfoMessage_(QString&)));

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::RtuClient(const QString& portName,
                   QSerialPort::BaudRate baudRate,
                   QSerialPort::DataBits dataBits,
                   QObject *parent)
    : Client(parent),
      RTUDevice(portName, baudRate, dataBits),
      inSilenceState_(false),
      silenceTimer_(this)
{
    ioDevice_ = serialPort_;

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    connect(this, SIGNAL(unitDebugMessage_(QString&)), this, SLOT(onUnitDebugMessage_(QString&)));
    connect(this, SIGNAL(unitErrorMessage_(QString&)), this, SLOT(onUnitErrorMessage_(QString&)));
    connect(this, SIGNAL(unitInfoMessage_(QString&)), this, SLOT(onUnitInfoMessage_(QString&)));

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::RtuClient(const QString& portName,
                   QSerialPort::BaudRate baudRate,
                   QObject *parent)
    : Client(parent),
      RTUDevice(portName, baudRate),
      inSilenceState_(false),
      silenceTimer_(this)
{
    ioDevice_ = serialPort_;

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    connect(this, SIGNAL(unitDebugMessage_(QString&)), this, SLOT(onUnitDebugMessage_(QString&)));
    connect(this, SIGNAL(unitErrorMessage_(QString&)), this, SLOT(onUnitErrorMessage_(QString&)));
    connect(this, SIGNAL(unitInfoMessage_(QString&)), this, SLOT(onUnitInfoMessage_(QString&)));

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::RtuClient(const QString& portName,
                   QObject *parent)
    : Client(parent),
      RTUDevice(portName),
      inSilenceState_(false),
      silenceTimer_(this)
{
    ioDevice_ = serialPort_;

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    connect(this, SIGNAL(unitDebugMessage_(QString&)), this, SLOT(onUnitDebugMessage_(QString&)));
    connect(this, SIGNAL(unitErrorMessage_(QString&)), this, SLOT(onUnitErrorMessage_(QString&)));
    connect(this, SIGNAL(unitInfoMessage_(QString&)), this, SLOT(onUnitInfoMessage_(QString&)));

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

bool
RtuClient::configurePort_()
{
    bool result = RTUDevice::configurePort_();

    if (!result)
    {
        emit errorMessage(lastErrorMessage());
    }

    return result;
}

//-----------------------------------------------------------------------------

void
RtuClient::onUnitDebugMessage_(const QString& msg)
{
    emit debugMessage(QString("[UnitID: %1] %2").arg(unitID_).arg(msg));
}

//-----------------------------------------------------------------------------

void
RtuClient::onUnitErrorMessage_(const QString& msg)
{
    emit errorMessage(QString("[UnitID: %1] %2").arg(unitID_).arg(msg));
}

//-----------------------------------------------------------------------------

void
RtuClient::onUnitInfoMessage_(const QString& msg)
{
    emit infoMessage(QString("[UnitID: %1] %2").arg(unitID_).arg(msg));
}

//-----------------------------------------------------------------------------

bool
RtuClient::sendRequestToServer_(const ProtocolDataUnit& requestPDU, int requestPDUSize)
{
    QByteArray adu = prepareADU_(requestPDU, requestPDUSize);

    return sendDataToServer_(adu);
}

//-----------------------------------------------------------------------------

bool
RtuClient::readResponseFromServer_(ProtocolDataUnit& pdu)
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

bool
RtuClient::openPort()
{
    bool result = RTUDevice::openPort();

    if (!result)
    {
        emit errorMessage(lastErrorMessage());
    }

    return result;
}

//-----------------------------------------------------------------------------

bool
RtuClient::sendDataToServer_(const QByteArray& request)
{
    if (inSilenceState_)
    {
#ifdef DEBUG
        qDebug() << "In silence state! Waiting...";
        emit debugMessage("In silence state! Waiting...");
#endif
        wait(silenceTime_);

#ifdef DEBUG
        qDebug() << "Waiting's been finised";
        emit debugMessage("Waiting's been finised");
#endif
    }

    if (!serialPort_->isOpen() && !openPort())
    {
        emit infoMessage(tr("Cannot open serial port!"));

        return false;
    }

    bool result = Client::sendDataToServer_(request);

    startSilence_();

    return  result;
}

//-----------------------------------------------------------------------------

void
RtuClient::startSilence_()
{
    inSilenceState_ = true;
    silenceTimer_.singleShot(silenceTime_, this, SLOT(stopSilence_()));
}

//-----------------------------------------------------------------------------

void
RtuClient::stopSilence_()
{
    inSilenceState_ = false;
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt
