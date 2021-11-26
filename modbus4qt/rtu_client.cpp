/*****************************************************************************
 * modbus4qt Library
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 * Copyright (C) 2012-2015
 * http://www.modbus4qt.ru
 *
 * $Id: rtu_client.cpp 154 2015-10-20 11:11:12Z l.kolesnik $
 * $URL: https://o.m-i.ru/svn/modbus4qt/branches/1.1/src/rtu_client.cpp $
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

*/

#include "rtu_client.h"
#include "utils.h"

#include <QDebug>
#include <QMutex>
#include <QWaitCondition>

namespace modbus4qt
{

RtuClient::RtuClient(const QString& portName,
                   QSerialPort::BaudRate baudRate,
                   QSerialPort::DataBits dataBits,
                   QSerialPort::StopBits stopBits,
                   QSerialPort::Parity parity,
                   QObject *parent)
    : Client(parent),
      portName_(portName),
      baudRate_(baudRate),
      dataBits_(dataBits),
      stopBits_(stopBits),
      parity_(parity),
      silenceTimer_(this),
      inSilenceState_(false),
      silenceTime_(0)
{
    ioDevice_ = new QSerialPort(this);
    serialPort_ = dynamic_cast<QSerialPort*>(ioDevice_);

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    serialPort_->setPortName(portName_);

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::RtuClient(const QString& portName,
                   QSerialPort::BaudRate baudRate,
                   QSerialPort::DataBits dataBits,
                   QSerialPort::StopBits stopBits,
                   QObject *parent)
    : Client(parent),
      portName_(portName),
      baudRate_(baudRate),
      dataBits_(dataBits),
      stopBits_(stopBits),
      parity_(QSerialPort::EvenParity),
      silenceTimer_(this),
      inSilenceState_(false),
      silenceTime_(0)
{
    ioDevice_ = new QSerialPort(this);
    serialPort_ = dynamic_cast<QSerialPort*>(ioDevice_);

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    serialPort_->setPortName(portName_);

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::RtuClient(const QString& portName,
                   QSerialPort::BaudRate baudRate,
                   QSerialPort::DataBits dataBits,
                   QObject *parent)
    : Client(parent),
      portName_(portName),
      baudRate_(baudRate),
      dataBits_(dataBits),
      stopBits_(QSerialPort::OneStop),
      parity_(QSerialPort::EvenParity),
      silenceTimer_(this),
      inSilenceState_(false),
      silenceTime_(0)
{
    ioDevice_ = new QSerialPort(this);
    serialPort_ = dynamic_cast<QSerialPort*>(ioDevice_);

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    serialPort_->setPortName(portName_);

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::RtuClient(const QString& portName,
                   QSerialPort::BaudRate baudRate,
                   QObject *parent)
    : Client(parent),
      portName_(portName),
      baudRate_(baudRate),
      dataBits_(QSerialPort::Data8),
      stopBits_(QSerialPort::OneStop),
      parity_(QSerialPort::EvenParity),
      silenceTimer_(this),
      inSilenceState_(false),
      silenceTime_(0)
{
    ioDevice_ = new QSerialPort(this);
    serialPort_ = dynamic_cast<QSerialPort*>(ioDevice_);

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    serialPort_->setPortName(portName_);

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::RtuClient(const QString& portName,
                   QObject *parent)
    : Client(parent),
      portName_(portName),
      baudRate_(QSerialPort::Baud9600),
      dataBits_(QSerialPort::Data8),
      stopBits_(QSerialPort::OneStop),
      parity_(QSerialPort::EvenParity),
      silenceTimer_(this),
      inSilenceState_(false),
      silenceTime_(0)
{
    ioDevice_ = new QSerialPort(this);
    serialPort_ = dynamic_cast<QSerialPort*>(ioDevice_);

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    serialPort_->setPortName(portName_);

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::~RtuClient()
{
    serialPort_->close();
}

//-----------------------------------------------------------------------------

bool
RtuClient::configurePort_()
{
    if (!serialPort_->isOpen())
    {
        emit errorMessage(tr("Cannot configure port %1. The port is not open!").arg(portName_));
        return false;
    }

    if (!serialPort_->setBaudRate(baudRate_))
    {
        emit errorMessage(serialPort_->errorString());
        return false;
    }
    setSilenceTime_();

    if (!serialPort_->setDataBits(dataBits_))
    {
        emit errorMessage(serialPort_->errorString());
        return false;
    }

    if (!serialPort_->setStopBits(stopBits_))
    {
        emit errorMessage(serialPort_->errorString());
        return false;
    }

    if (!serialPort_->setParity(parity_))
    {
        emit errorMessage(serialPort_->errorString());
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

bool
RtuClient::openPort()
{
    if (serialPort_->isOpen()) serialPort_->close();

    bool result = serialPort_->open(QIODevice::ReadWrite);

    if (!result)
    {
        emit errorMessage(serialPort_->errorString());
        serialPort_->close();
        return false;
    }
    else
    {
        qDebug() << "Port opened!";
        emit infoMessage(tr("Port opened!"));

        result = configurePort_();

        if (!result)
            serialPort_->close();
        else
        {
            qDebug() << "Port configured!";
            emit infoMessage(tr("Port configured!"));
        }
    }

    if (!result)
    {
        qDebug() << "Error while opening and configuring port!";
        emit errorMessage(tr("Error while opening and configuring port!"));
    }

    return result;
}

//-----------------------------------------------------------------------------

/*
 * <------------------------ MODBUS SERIAL LINE PDU (1) ------------------->
 *              <----------- MODBUS PDU (1') ---------------->
 *  +-----------+---------------+----------------------------+-------------+
 *  | Address   | Function Code | Data                       | CRC/LRC     |
 *  +-----------+---------------+----------------------------+-------------+
 *  |           |               |                                   |
 * (2)        (3/2')           (3')                                (4)
 *
 * (1)  ... MB_SER_PDU_SIZE_MAX = 256
 * (2)  ... MB_SER_PDU_ADDR_OFF = 0
 * (3)  ... MB_SER_PDU_PDU_OFF  = 1
 * (4)  ... MB_SER_PDU_SIZE_CRC = 2
 *
 * (1') ... MB_PDU_SIZE_MAX     = 253
 * (2') ... MB_PDU_FUNC_OFF     = 0
 * (3') ... MB_PDU_DATA_OFF     = 1
 */

QByteArray
RtuClient::prepareADU_(const ProtocolDataUnit &pdu, int pduSize)
{
    QByteArray result;

    result[0] = unitID_;

    result.insert(1, (char*)&pdu, pduSize);

    quint16 crc = host2net(crc16(result));
    result.append((char*)&crc, 2);

    qDebug() << "ADU: " << result.toHex();
    qDebug() << "ADU size: " << result.size();

    return result;
}

//-----------------------------------------------------------------------------

ProtocolDataUnit
RtuClient::processADU_(const QByteArray &buf)
{
    qDebug() << "ADU: " << buf.toHex();
    qDebug() << "ADU size: " << buf.size();

    QByteArray tempBuf(buf);

    // For any case we should check size of recieved data to protect memory
    //
    // На всякий случай проверяем размер полученных данных, чтобы не ничего не испортить
    //
    if (tempBuf.size() > int(sizeof(RTUApplicationDataUnit)))
        tempBuf.resize(sizeof(RTUApplicationDataUnit));

    int tempBufSize = tempBuf.size();

    // Minimum ADU size can be 5 bytes: 1 byte for address, 2 bytes for minimum PDU, 2 bytes for CRC
    //
    // Минимальный размер ADU может быть 5 байт: 1 байт адрес, 2 байта PDU и 2 байта CRC
    //
    if (tempBufSize < 5)
    {
        emit errorMessage(unitID_, tr("Wrong application data unit recieved!"));
        tempBuf.resize(5);
        tempBufSize = 5;
    }

    RTUApplicationDataUnit adu;

    adu.unitId = tempBuf[0];
    adu.pdu.functionCode = tempBuf[1];

    for (int i = 2; i < tempBufSize - 2; ++i)
        adu.pdu.data[i - 2] = tempBuf[i];

    WordRec aduCrc;
    aduCrc.bytes[0] = tempBuf[tempBufSize - 2];
    aduCrc.bytes[1] = tempBuf[tempBufSize - 1];

    adu.crc = aduCrc.word;

    quint16 crc = net2host(adu.crc);

    qDebug() << QString("recievied crc: %1").arg(crc, 4, 16);

    // Calculate CRC for recived packet
    // At first we should delete last 2 bytes, which is the CRC
    //
    // Высчитываем контрольную сумму для полученного пакета
    // Сначала для этого удаляем последние 2 байта, которые и содержат контрольную сумму
    //
    tempBuf.resize(tempBuf.size() - 2);
    quint16 crcCalc = crc16(tempBuf);

    qDebug() << QString("calculated crc: %1").arg(crcCalc, 4, 16);

    // In case of mismatch emit error message
    //
    // Если она не сходится, выдаем сообщение об ошибке
    //
    if (crc != crcCalc) emit errorMessage(unitID_, tr("CRC mismatch!"));

#ifndef QT_NO_DEBUG
    // If we want to print PDU into log file we should remove 1st byte
    // After that we have pdu in tempBuf as CRC was deleted before
    //
    // Чтобы вывести в лог PDU, удаляем 1-й байт
    // Теперь в tempBuf остался только pdu, так как CRC удалили ранее
    //
    tempBuf.remove(0, 1);
    qDebug() << "PDU: " << tempBuf.toHex();
    qDebug() << "PDU size: " << tempBuf.size();
#endif

    return adu.pdu;
}

//-----------------------------------------------------------------------------

QByteArray
RtuClient::readResponse_()
{
    QByteArray inArray;
    inArray.append(ioDevice_->readAll());
    while (ioDevice_->bytesAvailable() || ioDevice_->waitForReadyRead(silenceTime_ * 2))
    {
        inArray.append(ioDevice_->readAll());
    }
    return inArray;
}

//-----------------------------------------------------------------------------

bool
RtuClient::sendRequestToServer_(const ProtocolDataUnit &requestPDU, int requestPDUSize, ProtocolDataUnit *responsePDU)
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
        return false;
    }

    bool result = Client::sendRequestToServer_(requestPDU, requestPDUSize, responsePDU);

    startSilence_();

    return  result;
}

//-----------------------------------------------------------------------------

void
RtuClient::setBaudRate(QSerialPort::BaudRate baudRate)
{
    if (baudRate_ != baudRate)
    {
        baudRate_ = baudRate;
        setSilenceTime_();
        if (serialPort_->isOpen()) serialPort_->setBaudRate(baudRate_);
    }
}

//-----------------------------------------------------------------------------

void
RtuClient::setDataBits(QSerialPort::DataBits dataBits)
{
    if (dataBits_ != dataBits)
    {
        dataBits_ = dataBits;
        if (serialPort_->isOpen()) serialPort_->setDataBits(dataBits_);
    }
}

//-----------------------------------------------------------------------------

void
RtuClient::setParity(QSerialPort::Parity parity)
{
    if (parity_ != parity)
    {
        parity_ = parity;
        if (serialPort_->isOpen()) serialPort_->setParity(parity_);

    }
}

//-----------------------------------------------------------------------------

void
RtuClient::setPortName(const QString& portName)
{
    if (portName != portName_)
    {
        if (serialPort_->isOpen()) serialPort_->close();
        portName_ = portName;
        serialPort_->setPortName(portName_);
    }
}

//-----------------------------------------------------------------------------

QString
RtuClient::portName() const
{
    return portName_;
}

//-----------------------------------------------------------------------------

void RtuClient::setSilenceTime_()
{
    if (baudRate_ > QSerialPort::Baud19200)
        silenceTime_ = 2; // 2 ms > 1750 microseconds
    else
        // For each byte we should transmit 11 bits
        // We have to calculate time in seconds,
        // power it by 1000 and add 1 for any cases
        //
        // Для передачи каждого байта требуется 11 бит
        // Высчитываем время в секундах
        // Умножаем на 1000 и на всякий случай прибавляем единицу
        silenceTime_ = ((11.0 / baudRate_ ) * 3.5) * 1000 + 1;

    qDebug() << QString("Silence time: %1 ms").arg(silenceTime_);
    emit infoMessage(tr("Silence time: %1 ms").arg(silenceTime_));
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

void
RtuClient::setStopBits(QSerialPort::StopBits stopBits)
{
    if (stopBits_ != stopBits)
    {
        stopBits_ = stopBits;
        if (serialPort_->isOpen()) serialPort_->setStopBits(stopBits_);
    }
}

} // namespace modbus4qt
