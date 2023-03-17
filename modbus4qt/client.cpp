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

#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QVector>

#include <QDebug>

#include "memory_utils.h"
#include "client.h"

namespace modbus4qt
{

Client::Client(QObject* parent) :
    QObject(parent),
    ioDevice_(NULL),
    readTimeout_(DEFAULT_TIMEOUT),
    writeTimeout_(DEFAULT_TIMEOUT)
{
}

//-----------------------------------------------------------------------------

bool
Client::modbusTransaction(const Device::ProtocolDataUnit& requestPDU, int requestPDUSize, Device::ProtocolDataUnit& responsePDU)
{
    if (!sendRequestToServer_(requestPDU, requestPDUSize))
    {
        return false;
    }

    if (readResponseFromServer_(responsePDU) && (requestPDU.functionCode == responsePDU.functionCode))
    {
        return true;
    }
    else
    {
        uint8_t functionCode = static_cast<uint8_t>(requestPDU.functionCode);

        if ((functionCode | 0x80) == functionCode)
        {
            switch (functionCode - 0x80)
            {
                case static_cast<uint8_t>(Device::Exceptions::ILLEGAL_FUNCTION) :
                    emit errorMessage(tr("Illegal function!"));
                break;
                case static_cast<uint8_t>(Device::Exceptions::ILLEGAL_DATA_ADDRESS) :
                    emit errorMessage(tr("Illegal data address"));
                break;
                case static_cast<uint8_t>(Device::Exceptions::ILLEGAL_DATA_VALUE) :
                    emit errorMessage(tr("Illegal data value!"));
                break;
                case static_cast<uint8_t>(Device::Exceptions::SERVER_DEVICE_FAILURE) :
                    emit errorMessage(tr("Server device failure!"));
                break;
                case static_cast<uint8_t>(Device::Exceptions::ACKNOWLEDGE) :
                    emit errorMessage(tr("Acknowledge!"));
                break;
                case static_cast<uint8_t>(Device::Exceptions::SERVER_DEVICE_BUSY) :
                    emit errorMessage(tr("Server device busy!"));
                break;
                case static_cast<uint8_t>(Device::Exceptions::MEMORY_PARITY_ERROR) :
                    emit errorMessage(tr("Memory parity error!"));
                break;
                case static_cast<uint8_t>(Device::Exceptions::GATEWAY_PATH_NOT_AVAILABLE) :
                    emit errorMessage(tr("Gateway path not available!"));
                break;
                case static_cast<uint8_t>(Device::Exceptions::GATEWAY_TARGET_DEVICE_FAILED_TO_RESPONSE) :
                    emit errorMessage(tr("Gateway target device failed to response!"));
                break;
                default :
                    emit errorMessage(tr("Unknown error!"));
            }
        }
        else
        {
            emit errorMessage(tr("Response mismatch!"));
        }
        return false;
    }
}

//-----------------------------------------------------------------------------

bool
Client::modbusTransaction(const Device::ProtocolDataUnit& requestPDU, int requestPDUSize)
{
    Device::ProtocolDataUnit responsePDU;
    return modbusTransaction(requestPDU, requestPDUSize, responsePDU);
}

//-----------------------------------------------------------------------------

bool
Client::readCoil(quint16 regNo, bool& value)
{
    QVector<bool> data(1);

    bool result = readCoils(regNo, 1, data);
    value = data[0];

    return result;
}

//-----------------------------------------------------------------------------

bool
Client::readCoils(uint16_t regStart, uint16_t regQty, QVector<bool>& values)
{
    Device::ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Device::Functions::READ_COILS;

    // Start address
    //
    requestPDU.data[0] = hi(regStart);
    requestPDU.data[1] = low(regStart);

    if (regQty > MAX_COILS_FOR_READ)
    {
        regQty = MAX_COILS_FOR_READ;
    }

    // Quantity of registers
    //
    requestPDU.data[2] = hi(regQty);
    requestPDU.data[3] = low(regQty);

    requestPDUSize = 5;

    Device::ProtocolDataUnit responsePDU;

    bool isOk = modbusTransaction(requestPDU, requestPDUSize, responsePDU);

    if (isOk)
    {
        // Quantity of readed bytes, not coils!
        //
        int bytesReaded = responsePDU.data[0];

        // Copy data from pdu to buffer for process
        //
        QByteArray coilsBuffer((const char*)responsePDU.data + 1, bytesReaded);

        // Process buffer and read coils values from it
        //
        values = Device::getCoilsFromBuffer(coilsBuffer, regQty);
    }

    return isOk;
}

//-----------------------------------------------------------------------------

bool
Client::readDataFromServer_(QByteArray& response)
{
    if (!ioDevice_->waitForReadyRead(readTimeout_))
    {
        emit errorMessage(tr("Read timeout, error: %1").arg(ioDevice_->errorString()));

        return false;
    }

    response.clear();

    response.append(ioDevice_->readAll());
    while (ioDevice_->bytesAvailable() || ioDevice_->waitForReadyRead(readTimeout_))
    {
        response.append(ioDevice_->readAll());
    }

    return true;
}

//-----------------------------------------------------------------------------

bool
Client::readDescreteInput(uint16_t regNo, bool& value)
{
    QVector<bool> data(1);

    bool result = readDescreteInputs(regNo, 1, data);
    value = data[0];

    return result;
}

//-----------------------------------------------------------------------------

bool
Client::readDescreteInputs(uint16_t regStart, uint16_t regQty, QVector<bool>& values)
{
    Device::ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Device::Functions::READ_DESCRETE_INPUTS;

    // Start address
    //
    requestPDU.data[0] = hi(regStart);
    requestPDU.data[1] = low(regStart);

    if (regQty > MAX_COILS_FOR_READ)
    {
        regQty = MAX_COILS_FOR_READ;
    }

    // Quantity of registers
    //
    requestPDU.data[2] = hi(regQty);
    requestPDU.data[3] = low(regQty);

    requestPDUSize = 5;

    Device::ProtocolDataUnit responsePDU;

    bool isOk = modbusTransaction(requestPDU, requestPDUSize, responsePDU);

    if (isOk)
    {
        // Quantity of readed bytes, not registers!
        //
        int bytesReaded = responsePDU.data[0];

        // Copy data from pdu to buffer for process
        //
        QByteArray coilsBuffer((const char*)responsePDU.data + 1, bytesReaded);

        // Process buffer and read coils values from it
        //
        values = Device::getCoilsFromBuffer(coilsBuffer, regQty);
    }

    return isOk;
}


//-----------------------------------------------------------------------------

bool
Client::readHoldingRegister(uint16_t regNo, quint16& value)
{
    QVector<quint16> data(1);

    bool result = readHoldingRegisters(regNo, 1, data);
    value = data[0];

    return result;
}

//-----------------------------------------------------------------------------

bool
Client::readHoldingRegisters(uint16_t regStart, uint16_t regQty, QVector<uint16_t>& values)
{
    Device::ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Device::Functions::READ_HOLDING_REGISTERS;

    // Start address
    //
    requestPDU.data[0] = hi(regStart);
    requestPDU.data[1] = low(regStart);

    if (regQty > MAX_REGISTERS_FOR_READ)
    {
        emit infoMessage(tr("Maxixmum registers quantity (%1 items) for reading exceeded.\n"
                            "Only %1 items will be readed!").arg(MAX_REGISTERS_FOR_READ));

        regQty = MAX_REGISTERS_FOR_READ;
    }

    // Quantity of registers
    //
    requestPDU.data[2] = hi(regQty);
    requestPDU.data[3] = low(regQty);

    requestPDUSize = 5;

    Device::ProtocolDataUnit responsePDU;

    bool isOk = modbusTransaction(requestPDU, requestPDUSize, responsePDU);

    if (isOk)
    {
        // Quantity of readed bytes, not registers!
        //
        int bytesReaded = responsePDU.data[0];

        // Copy data from pdu to buffer for process
        //
        QByteArray registersBuffer((const char*)responsePDU.data + 1, bytesReaded);

        // Process buffer and read registers values from it
        //
        values = Device::getRegistersFromBuffer(registersBuffer, regQty);
    }

    return isOk;
}

//-----------------------------------------------------------------------------

bool
Client::readInputRegister(uint16_t regNo, uint16_t& value)
{
    QVector<quint16> data(1);
    bool result = false;

    result = readInputRegisters(regNo, 1, data);
    value = data[0];

    return result;
}

//-----------------------------------------------------------------------------

bool
Client::readInputRegisters(uint16_t regStart, uint16_t regQty, QVector<quint16>& values)
{
    Device::ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Device::Functions::READ_INPUT_REGISTERS;

    // Start address
    //
    requestPDU.data[0] = hi(regStart);
    requestPDU.data[1] = low(regStart);

    if (regQty > MAX_REGISTERS_FOR_READ)
    {
        emit infoMessage(tr("Maxixmum registers quantity (%1 items) for reading exceeded.\n"
                            "Only %1 items will be readed!").arg(MAX_REGISTERS_FOR_READ));

        regQty = MAX_REGISTERS_FOR_READ;
    }

    // Quantity of registers
    //
    requestPDU.data[2] = hi(regQty);
    requestPDU.data[3] = low(regQty);

    requestPDUSize = 5;

    Device::ProtocolDataUnit responsePDU;

    bool isOk = modbusTransaction(requestPDU, requestPDUSize, responsePDU);

    if (isOk)
    {
        // Quantity of readed bytes, not registers!
        //
        int bytesReaded = responsePDU.data[0];

        // Copy data from pdu to buffer for process
        //
        QByteArray registersBuffer((const char*)responsePDU.data + 1, bytesReaded);

        // Process buffer and read registers values from it
        //
        values = Device::getRegistersFromBuffer(registersBuffer, regQty);
    }

    return isOk;
}

//-----------------------------------------------------------------------------

int
Client::readTimeout() const
{
    return readTimeout_;
}

//-----------------------------------------------------------------------------

void
Client::setReadTimeOut(int readTimeout)
{
    readTimeout_ = readTimeout;
}

//-----------------------------------------------------------------------------

void
Client::setWriteTimeOut(int writeTimeout)
{
    writeTimeout_ = writeTimeout;
}

//-----------------------------------------------------------------------------

bool
Client::userDefinedFunction(uint8_t function, const QVector<uint8_t>& data, QVector<uint8_t>& retData)
{
    Device::ProtocolDataUnit pdu;

    int pduSize = 0;

    pdu.functionCode = function;

    for (quint16 i = 0; (i < data.size()) && (i < PDU_DATA_MAX_SIZE); ++i)
    {
        pdu.data[i] = data[i];
    }

    pduSize = 1 + data.size();

    Device::ProtocolDataUnit replyPdu;

    bool isOk = modbusTransaction(pdu, pduSize, replyPdu);

    if (!isOk)
    {
        return false;
    }
    else
    {
        retData.clear();

        for (int i = 0; i < PDU_DATA_MAX_SIZE; ++i)
        {
            retData.append(pdu.data[i]);
        }

        return true;
    }
}

//-----------------------------------------------------------------------------

bool
Client::userDefinedFunction(uint8_t function, uint8_t subFunction, const QVector<uint8_t>& data, QVector<uint8_t>& retData)
{
    Device::ProtocolDataUnit pdu;

    int pduSize = 0;

    pdu.functionCode = function;
    pdu.data[0] = subFunction;

    for (quint16 i = 0; (i < data.size()) && (i < (PDU_DATA_MAX_SIZE - 1)); ++i)
    {
        pdu.data[i + 1] = data[i];
    }

    pduSize = 2 + data.size();

    Device::ProtocolDataUnit responsePDU;

    bool isOk = modbusTransaction(pdu, pduSize, responsePDU);

    if (!isOk)
    {
        return false;
    }
    else
    {
        retData.clear();

        for (int i = 0; i < PDU_DATA_MAX_SIZE; ++i)
        {
            retData.append(responsePDU.data[i]);
        }

        return true;
    }
}

//-----------------------------------------------------------------------------

bool Client::writeCoil(uint16_t regNo, bool value)
{
    Device::ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Device::Functions::WRITE_SINGLE_COIL;

    // Coil address
    //
    requestPDU.data[0] = hi(regNo);
    requestPDU.data[1] = low(regNo);

    // 0xFF00 - on, 0x0000 - off
    // All other values will be ignored
    // See: Modbus Protocol Specification v1.1b3, p. 17

    if (value)
    {
        requestPDU.data[2] = 255;
    }
    else
    {
        requestPDU.data[2] = 0;
    }

    requestPDU.data[3] = 0;
    requestPDUSize = 5;

    return modbusTransaction(requestPDU, requestPDUSize);
}

//-----------------------------------------------------------------------------

bool
Client::writeCoils(uint16_t regStart, const QVector<bool>& values)
{
    Device::ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Device::Functions::WRITE_MULTIPLE_COILS;

    // Start address
    //
    requestPDU.data[0] = hi(regStart);
    requestPDU.data[1] = low(regStart);

    int regQty = values.size();

    if (regQty > MAX_COILS_FOR_WRITE)
    {
        emit infoMessage(tr("Maxixmum coils quantity (%1 items) for writing exceeded.\n"
                            "Only %1 items will be writed!").arg(MAX_COILS_FOR_WRITE));

        regQty = MAX_COILS_FOR_WRITE;
    }

    // Quantity of values to write
    //
    requestPDU.data[2] = hi(regQty);
    requestPDU.data[3] = low(regQty);

    // Bytes needed for values to write
    //
    requestPDU.data[4] = (regQty + 7) / 8;

    // Values
    //
    Device::putCoilsIntoBuffer(values, requestPDU.data + 5);

    // PDU size: 6 bytes + bytes needed for values to write
    //
    requestPDUSize = 6 + requestPDU.data[4];

    return modbusTransaction(requestPDU, requestPDUSize);
}


//-----------------------------------------------------------------------------

bool
Client::sendDataToServer_(const QByteArray& request)
{
    int64_t bytesWritten = ioDevice_->write(request);

    if (bytesWritten <= 0)
    {
        emit errorMessage(tr("Failed to write data, error: %1").arg(ioDevice_->errorString()));
        return false;
    }
    else if (bytesWritten < request.size())
    {
        emit errorMessage(tr("Failed to write all data, error: %1").arg(ioDevice_->errorString()));
        return false;
    }
    else if (!ioDevice_->waitForBytesWritten(writeTimeout_))
    {
        qDebug() << "Write timeout!";
        emit errorMessage(tr("Write timeout, error: %1").arg(ioDevice_->errorString()));
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

bool
Client::writeHoldingRegister(uint16_t regAddress, uint16_t value)
{
    Device::ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Device::Functions::WRITE_SINGLE_REGISTER;

    // Register address
    //
    requestPDU.data[0] = hi(regAddress);
    requestPDU.data[1] = low(regAddress);

    // Register value
    //
    requestPDU.data[2] = hi(value);
    requestPDU.data[3] = low(value);

    requestPDUSize = 5;

    return modbusTransaction(requestPDU, requestPDUSize);
}

//-----------------------------------------------------------------------------

bool
Client::writeHoldingRegisters(uint16_t regStart, const QVector<uint16_t>& values)
{
    Device::ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Device::Functions::WRITE_MULTIPLE_REGISTERS;

    // Start address
    //
    requestPDU.data[0] = hi(regStart);
    requestPDU.data[1] = low(regStart);

    int regQty = values.size();

    if (regQty > MAX_REGISTERS_FOR_WRITE)
    {
        emit infoMessage(tr("Maxixmum holding registers quantity (%1 items) for writing exceeded.\n"
                            "Only %1 items will be writed!").arg(MAX_REGISTERS_FOR_WRITE));

        regQty = MAX_REGISTERS_FOR_WRITE;
    }

    // Quantity of values to write
    //
    requestPDU.data[2] = hi(regQty);
    requestPDU.data[3] = low(regQty);

    // Bytes needed for values to write
    //
    requestPDU.data[4] = regQty * 2;

    // Values
    //
    Device::putRegistersIntoBuffer(values, requestPDU.data + 5);

    // PDU size: 6 bytes + bytes needed for values to write
    //
    requestPDUSize = 6 + requestPDU.data[4];

    return modbusTransaction(requestPDU, requestPDUSize);
}

//-----------------------------------------------------------------------------

int
Client::writeTimeout() const
{
    return writeTimeout_;
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt
