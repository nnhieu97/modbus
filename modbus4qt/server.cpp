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
#include <memory>
#include "device.h"
#include "server_internal_data.h"
#include "server.h"
#include "memory_utils.h"


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

    // Validate data address. If not valid (ExceptionCode = 2;) Send modbus exception response.
    QByteArray response;

    // Validate function code. If not valid (ExceptionCode = 1;) Send modbus exception response.
    using FNC = modbus4qt::Device::Functions;
    const QVector<uint8_t> avaliableCodes = {FNC::READ_COILS, FNC::READ_DESCRETE_INPUTS,
                                            FNC::READ_HOLDING_REGISTERS, FNC::READ_INPUT_REGISTERS,
                                            FNC::WRITE_SINGLE_COIL, FNC::WRITE_SINGLE_REGISTER,
                                            FNC::WRITE_MULTIPLE_COILS, FNC::WRITE_MULTIPLE_REGISTERS};
    if (std::find(avaliableCodes.cbegin(), avaliableCodes.cend(), requestPDU.functionCode) != avaliableCodes.cend()){
        // prepere response pdu
        // send it to client
        switch(requestPDU.functionCode)
        {
            case FNC::READ_COILS:
            case FNC::WRITE_SINGLE_COIL:
            case FNC::WRITE_MULTIPLE_COILS:
                if (internalData_->coils()->isEmpty())
                {
                    response.append((const char*)(Device::Exceptions::ILLEGAL_FUNCTION));
                    return sendDataToClient_(response);
                }
            break;
            case FNC::READ_DESCRETE_INPUTS:
                if (internalData_->descreteInputs().isEmpty())
                {
                    response.append((const char*)(Device::Exceptions::ILLEGAL_FUNCTION));
                    return sendDataToClient_(response);
                }
            break;
            case FNC::READ_HOLDING_REGISTERS:
                if (internalData_->holdingRegisters().isEmpty())
                {
                    response.append((const char*)(Device::Exceptions::ILLEGAL_FUNCTION));
                    return sendDataToClient_(response);
                }
            break;
            case FNC::READ_INPUT_REGISTERS:
                if (internalData_->inputRegisters().isEmpty())
                {
                    response.append((const char*)(Device::Exceptions::ILLEGAL_FUNCTION));
                    return sendDataToClient_(response);
                }
            break;
            default : ;// is empty
        }
    }
    else{
        response.append((const char*)(Device::Exceptions::ILLEGAL_FUNCTION));
        return false;
        //If not valid (ExceptionCode = 1;)
    }


    // Validate data value.  If not valid (ExceptionCode = 3;) Send modbus exception response.

    uint16_t startAddress = modbus4qt::net2host(*(const quint16*)requestPDU.data);
    uint16_t qty = modbus4qt::net2host(*((const quint16*)requestPDU.data) + 1);

    switch(requestPDU.functionCode)
    {
        case FNC::READ_COILS:
        {
            const QMap<quint16, bool> baseCoils = internalData_->constCoils();
            //first case
             if(!(baseCoils.contains(startAddress)))
             {
                 response.append((const char*)(Device::Exceptions::ILLEGAL_DATA_ADDRESS));
                 return sendDataToClient_(response);
             }
             //second case
             else if(!(baseCoils.contains(startAddress)))
             {
                 for ( uint16_t i = startAddress + 1; i < startAddress + qty; ++i) {
                     if(baseCoils.contains(i)){}
                     else
                     {
                         response.append((const char*)(Device::Exceptions::ILLEGAL_DATA_ADDRESS));
                         return sendDataToClient_(response);
                     }
                 }
             }
             //third case - nothing
        }
        break;

        case FNC::READ_DESCRETE_INPUTS:
        {
            const QMap<quint16, bool> baseDescreteInputs = internalData_->descreteInputs();
            //first case
             if(!(baseDescreteInputs.contains(startAddress)))
             {
                 response.append((const char*)(Device::Exceptions::ILLEGAL_DATA_ADDRESS));
                 return sendDataToClient_(response);
             }
             //second case
             else if(!(baseDescreteInputs.contains(startAddress)))
             {
                 for ( uint16_t i = startAddress + 1; i < startAddress + qty; ++i) {
                     if(baseDescreteInputs.contains(i)){}
                     else
                     {
                         response.append((const char*)(Device::Exceptions::ILLEGAL_DATA_ADDRESS));
                         return sendDataToClient_(response);
                     }
                 }
             }
        }
        break;

        case FNC::READ_HOLDING_REGISTERS:
        {
            const QMap<quint16, quint16> baseHoldingRegisters = internalData_->holdingRegisters();
            if(!(baseHoldingRegisters.contains(startAddress)))
            {
                response.append((const char*)(Device::Exceptions::ILLEGAL_DATA_ADDRESS));
                return sendDataToClient_(response);
            }
            //second case
            else if(!(baseHoldingRegisters.contains(startAddress)))
            {
                for ( uint16_t i = startAddress + 1; i < startAddress + qty; ++i) {
                    if(baseHoldingRegisters.contains(i)){}
                    else
                    {
                        response.append((const char*)(Device::Exceptions::ILLEGAL_DATA_ADDRESS));
                        return sendDataToClient_(response);
                    }
                }
            }
        }
        break;

        case FNC::READ_INPUT_REGISTERS:
        {
            const QMap<quint16, quint16> baseInputRegisters = internalData_->inputRegisters();
            if(!(baseInputRegisters.contains(startAddress)))
            {
                response.append((const char*)(Device::Exceptions::ILLEGAL_DATA_VALUE));
                return sendDataToClient_(response);
            }
            //second case
            else if(!(baseInputRegisters.contains(startAddress)))
            {
                for ( uint16_t i = startAddress + 1; i < startAddress + qty; ++i) {
                    if(baseInputRegisters.contains(i)){}
                    else
                    {
                        response.append((const char*)(Device::Exceptions::ILLEGAL_DATA_ADDRESS));
                        return sendDataToClient_(response);
                    }
                }
            }
        }
        break;

        default:;
    }

    // Execute MB function.  If not valid (ExceptionCode = 4, 5, 6;) Send modbus exception response.

    // regs
    //
    response.append(host2net(qty));
    switch (requestPDU.functionCode)
    {
        case FNC::READ_HOLDING_REGISTERS:
        {
           const QMap<quint16, quint16> baseHoldingRegisters = internalData_->holdingRegisters();
           for ( uint16_t i = startAddress; i < startAddress + qty; ++i)
           {
               response.append(host2net(baseHoldingRegisters[i]));
           }
        }
        break;

        case FNC::READ_INPUT_REGISTERS:
        {
            const QMap<quint16, quint16> baseInputRegisters = internalData_->inputRegisters();
            for ( uint16_t i = startAddress; i < startAddress + qty; ++i)
            {
                response.append(host2net(baseInputRegisters[i]));
            }
        }
        break;

        case FNC::READ_COILS:
        {
            const QMap<quint16, bool> baseCoils = internalData_->constCoils();
            QVector<bool> coilsVec;
            for (uint16_t i = startAddress; i < startAddress + qty; ++i)
            {
                coilsVec.append(baseCoils[i]);
            }
            const int size = coilsVec.size();
            uint8_t* buffer = new uint8_t[size];
            modbus4qt::Device::putCoilsIntoBuffer(coilsVec, buffer);
        }
        break;
        case FNC::READ_DESCRETE_INPUTS:
            {
                const QMap<quint16, bool> baseDescreteInputs = internalData_->descreteInputs();
                QVector<bool> coilsVec;
                for (uint16_t i = startAddress; i < startAddress + qty; ++i)
                {
                    coilsVec.append(baseDescreteInputs[i]);
                }
                const int size = coilsVec.size();
                uint8_t* buffer = new uint8_t[size];
                modbus4qt::Device::putCoilsIntoBuffer(coilsVec, buffer);
            }
        break;

        default:;

    // All's goos. Do some work & send modbus response
    //

    }
    return sendDataToClient_(response);
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
