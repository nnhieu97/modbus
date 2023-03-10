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

#include "client.h"
#include "utils.h"

#include <QDataStream>
#include <QIODevice>
#include <QVector>

#include <QDebug>

namespace modbus4qt
{

Client::Client(QObject *parent) :
    AbstractDevice(parent),
    ioDevice_(NULL),
    readTimeout_(DEFAULT_TIMEOUT),
    writeTimeout_(DEFAULT_TIMEOUT),
    unitID_(0)
{
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
Client::readCoils(quint16 regStart, quint16 regQty, QVector<bool>& values)
{
    ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Functions::ReadCoils;

    // Start address
    requestPDU.data[0] = hi(regStart);
    requestPDU.data[1] = lo(regStart);

    if (regQty > MaxCoilsForRead) regQty = MaxCoilsForRead;

    // Quantity of registers
    requestPDU.data[2] = hi(regQty);
    requestPDU.data[3] = lo(regQty);

    requestPDUSize = 5;

    ProtocolDataUnit responsePDU;

    bool isOk = sendRequestToServer_(requestPDU, requestPDUSize, &responsePDU);

    if (isOk)
    {
        // Quantity of readed bytes, not coils!
        int bytesReaded = responsePDU.data[0];

        // Copy data from pdu to buffer for process
        QByteArray coilsBuffer((const char*)responsePDU.data + 1, bytesReaded);
        qDebug() << "Coils buffer: " << coilsBuffer.toHex();

        // Process buffer and read coils values from it
        values = getCoilsFromBuffer(coilsBuffer, regQty);
    }

    return isOk;
}

//-----------------------------------------------------------------------------

bool
Client::readDescreteInputs(quint16 regStart, quint16 regQty, QVector<bool>& values)
{
    ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Functions::ReadDescereteInputs;

    // Start address
    requestPDU.data[0] = hi(regStart);
    requestPDU.data[1] = lo(regStart);

    if (regQty > MaxCoilsForRead) regQty = MaxCoilsForRead;

    // Quantity of registers
    requestPDU.data[2] = hi(regQty);
    requestPDU.data[3] = lo(regQty);

    requestPDUSize = 5;

    ProtocolDataUnit responsePDU;

    bool isOk = sendRequestToServer_(requestPDU, requestPDUSize, &responsePDU);

    if (isOk)
    {
        // Quantity of readed bytes, not coils!
        int bytesReaded = responsePDU.data[0];

        // Copy data from pdu to buffer for process
        QByteArray coilsBuffer((const char*)responsePDU.data + 1, bytesReaded);
        qDebug() << "Descrete inputs buffer: " << coilsBuffer.toHex();

        // Process buffer and read coils values from it
        values = getCoilsFromBuffer(coilsBuffer, regQty);
    }

    return isOk;
}

//-----------------------------------------------------------------------------

//bool
//Client::readDouble(quint16 regNo, double& value)
//{
//    QVector<quint16> buf(sizeof(value));

//    bool result = readHoldingRegisters(regNo, sizeof(value), buf);

//    if (result)
//    {
//        memcpy(buf.data(), &value, sizeof(value));
//    }
//    else
//    {
//        value = 0.0;
//    }

//    return result;
//}

//-----------------------------------------------------------------------------

//bool
//Client::readDWord(quint16 regNo, quint32& value)
//{
//    QVector<quint16> buf(sizeof(value));

//    bool result = readHoldingRegisters(regNo, sizeof(value), buf);

//    if (result)
//    {
//        memcpy(buf.data(), &value, sizeof(value));
//    }
//    else
//    {
//        value = 0;
//    }

//    return result;
//}

//-----------------------------------------------------------------------------

bool
Client::readHoldingRegister(quint16 regNo, quint16& value)
{
    QVector<quint16> data(1);

    bool result = readHoldingRegisters(regNo, 1, data);
    value = data[0];

    return result;
}

//-----------------------------------------------------------------------------

bool
Client::readHoldingRegisters(quint16 regStart, quint16 regQty, QVector<quint16>& values)
{
    ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Functions::ReadHoldingRegisters;

    // Start address
    requestPDU.data[0] = hi(regStart);
    requestPDU.data[1] = lo(regStart);

    if (regQty > MaxRegistersForRead)
    {
        emit infoMessage(tr("Maxixmum registers quantity (125 items) for reading exceeded. Only allowed quantity will be readed!"));
        regQty = MaxRegistersForRead;
    }

    // Quantity of registers
    //
    requestPDU.data[2] = hi(regQty);
    requestPDU.data[3] = lo(regQty);

    requestPDUSize = 5;

    ProtocolDataUnit responsePDU;

    bool isOk = sendRequestToServer_(requestPDU, requestPDUSize, &responsePDU);

    if (isOk)
    {
        // Quantity of readed bytes, not registers!
        //
        int bytesReaded = responsePDU.data[0];

        // Copy data from pdu to buffer for process
        //
        QByteArray registersBuffer((const char*)responsePDU.data + 1, bytesReaded);
        qDebug() << "Registers buffer: " << registersBuffer.toHex();

        // Process buffer and read registers values from it
        //
        values = getRegistersFromBuffer(registersBuffer, regQty);
    }

    return isOk;
}

//-----------------------------------------------------------------------------

bool
Client::readInputRegister(quint16 regNo, quint16& value)
{
    QVector<quint16> data(1);
    bool result = false;

    result = readInputRegisters(regNo, 1, data);
    value = data[0];

    return result;
}

//-----------------------------------------------------------------------------

bool
Client::readInputRegisters(quint16 regStart, quint16 regQty, QVector<quint16>& values)
{
    ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Functions::ReadInputRegisters;

    // Start address
    requestPDU.data[0] = hi(regStart);
    requestPDU.data[1] = lo(regStart);

    if (regQty > MaxRegistersForRead)
    {
        //emit infoMessage(tr("Maxixmum registers quantity for reading exceeded. Only allowed quantity will be readed!"));
        regQty = MaxRegistersForRead;
    }

    // Quantity of registers
    requestPDU.data[2] = hi(regQty);
    requestPDU.data[3] = lo(regQty);

    requestPDUSize = 5;

    ProtocolDataUnit responsePDU;

    bool isOk = sendRequestToServer_(requestPDU, requestPDUSize, &responsePDU);

    if (isOk)
    {
        // Quantity of readed bytes, not registers!
        //
        int bytesReaded = responsePDU.data[0];

        // Copy data from pdu to buffer for process
        //
        QByteArray registersBuffer((const char*)responsePDU.data + 1, bytesReaded);
        qDebug() << "Registers buffer: " << registersBuffer.toHex();

        // Process buffer and read registers values from it
        //
        values = getRegistersFromBuffer(registersBuffer, regQty);
    }

    return isOk;
}

//-----------------------------------------------------------------------------

//bool
//Client::readSingle(quint16 regNo, float& value /* Single*/)
//{
//    QVector<quint16> buf(sizeof(value));

//    bool result = readHoldingRegisters(regNo, sizeof(value), buf);

//    if (result)
//    {
//        memcpy(buf.data(), &value, sizeof(value));
//    }
//    else
//    {
//        value = 0;
//    }

//    return result;
//}

//-----------------------------------------------------------------------------

//QString
//Client::readString(quint16 /*regNo*/, quint16 /*length*/)
//{
///*
//    function TIdModbusClient.ReadString(const RegNo: Word; const ALength: Word): String;
//    var
//      BlockCount: Word;
//      Data: array of Word;
//      i: Integer;
//    begin
//      Result := '';
//      BlockCount := Round((ALength / 2) + 0.1);
//      SetLength(Data, BlockCount);
//      FillChar(Data[0], BlockCount, 0);

//      if ReadHoldingRegisters(RegNo, BlockCount, Data) then
//      begin
//        for i := 0 to (BlockCount - 1) do
//        begin
//          Result := Result + Chr(WordRec(Data[i]).Hi);
//          if (Length(Result) < ALength) then
//            Result := Result + Chr(WordRec(Data[i]).Lo);
//        end;
//      end;
//    end;
//*/
//    QString result;

//    return result;
//}

//-----------------------------------------------------------------------------

bool
Client::sendRequestToServer_(const ProtocolDataUnit& requestPDU, int requestPDUSize, ProtocolDataUnit* responsePDU)
{
    QByteArray adu = prepareADU_(requestPDU, requestPDUSize);

    qDebug() << "Sending data: " << adu.toHex();

    qint64 bytesWritten = ioDevice_->write(adu);
    if (bytesWritten <= 0)
    {
        emit errorMessage(tr("Failed to write data for unit %2, error: %1").arg(ioDevice_->errorString()).arg(unitID_));
        return false;
    }
    else if (bytesWritten < adu.size())
    {
        emit errorMessage(tr("Failed to write all data unit %2, error: %1").arg(ioDevice_->errorString()).arg(unitID_));
        return false;
    }
    else if (!ioDevice_->waitForBytesWritten(writeTimeout_))
    {
        qDebug() << "Write timeout!";
        emit errorMessage(tr("Write timeout for unit #%2, error: %1").arg(ioDevice_->errorString()).arg(unitID_));
        return false;
    }

    qDebug() << "Read timeout: " << readTimeout_ << " ms";

    bool result = ioDevice_->waitForReadyRead(readTimeout_);
    if (!result)
    {
        emit errorMessage(tr("Read timeout for unit #%2, error: %1").arg(ioDevice_->errorString()).arg(unitID_));
        return false;
    }

    QByteArray inArray = readResponse_();

    qDebug() << "Readed data: " << inArray.toHex();

    *responsePDU = processADU_(inArray);

    if (requestPDU.functionCode == responsePDU->functionCode)
    {
        return true;
    }
    else
    {
        if ((requestPDU.functionCode | 0x80) == responsePDU->functionCode)
        {
            switch (responsePDU->functionCode - 0x80)
            {
                case static_cast<quint8>(Exceptions::ILLEGAL_FUNCTION) :
                    emit errorMessage(tr("Illegal function for unit #%1!").arg(unitID_));
                break;
                case static_cast<quint8>(Exceptions::ILLEGAL_DATA_ADDRESS) :
                    emit errorMessage(tr("Illegal data address for unit #%1!").arg(unitID_));
                break;
                case static_cast<quint8>(Exceptions::ILLEGAL_DATA_VALUE) :
                    emit errorMessage(tr("Illegal data value for unit #%1!").arg(unitID_));
                break;
                case static_cast<quint8>(Exceptions::SERVER_DEVICE_FAILURE) :
                    emit errorMessage(tr("Server device failure for unit #%1!").arg(unitID_));
                break;
                case static_cast<quint8>(Exceptions::ACKNOWLEDGE) :
                    emit errorMessage(tr("Acknowledge for unit #%1!").arg(unitID_));
                break;
                case static_cast<quint8>(Exceptions::SERVER_DEVICE_BUSY) :
                    emit errorMessage(tr("Server device at #%1 busy!").arg(unitID_));
                break;
                case static_cast<quint8>(Exceptions::MEMORY_PARITY_ERROR) :
                    emit errorMessage(tr("Memory parity error in unit #%1!").arg(unitID_));
                break;
                case static_cast<quint8>(Exceptions::GATEWAY_PATH_NOT_AVAILABLE) :
                    emit errorMessage(tr("Gateway path not available for unit #%1!").arg(unitID_));
                break;
                case static_cast<quint8>(Exceptions::GATEWAY_TARGET_DEVICE_FAILED_TO_RESPONSE) :
                    emit errorMessage(tr("Gateway target device failed to response for unit #%1!").arg(unitID_));
                break;
                default :
                    emit errorMessage(tr("Unknown error for unit #%1!").arg(unitID_));
            }
        }
        else
        {
            emit errorMessage(tr("Response mismatch for unit #%1!").arg(unitID_));
        }
        return false;
    }
}

//-----------------------------------------------------------------------------

bool
Client::sendRequestToServer_(const ProtocolDataUnit &pdu, int pduSize)
{
    ProtocolDataUnit responsePDU;
    return sendRequestToServer_(pdu, pduSize, &responsePDU);
}

//-----------------------------------------------------------------------------

bool
Client::writeMultipleCoils(quint16 regStart, const QVector<bool>& values)
{
    ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Functions::WriteMultipleCoils;

    // Start address
    requestPDU.data[0] = hi(regStart);
    requestPDU.data[1] = lo(regStart);

    int regQty = values.size();
    if (regQty > MaxCoilsForWrite)
    {
        // emit infoMessage();
        regQty = MaxCoilsForWrite;
    }

    // Quantity of values to write
    requestPDU.data[2] = hi(regQty);
    requestPDU.data[3] = lo(regQty);

    // Bytes needed for values to write
    requestPDU.data[4] = (regQty + 7) / 8;

    // Values
    putCoilsIntoBuffer(requestPDU.data + 5, values);

    // PDU size: 6 bytes + bytes needed for values to write
    requestPDUSize = 6 + requestPDU.data[4];

    return sendRequestToServer_(requestPDU, requestPDUSize);
}

//-----------------------------------------------------------------------------

bool
Client::writeMultipleRegisters(quint16 regStart, const QVector<quint16> &values)
{
    ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Functions::WriteMultipleRegisters;

    // Start address
    requestPDU.data[0] = hi(regStart);
    requestPDU.data[1] = lo(regStart);

    int regQty = values.size();
    if (regQty > MaxRegistersForWrite)
    {
        // emit infoMessage();
        regQty = MaxRegistersForWrite;
    }

    // Quantity of values to write
    requestPDU.data[2] = hi(regQty);
    requestPDU.data[3] = lo(regQty);

    // Bytes needed for values to write
    requestPDU.data[4] = regQty * 2;

    // Values
    putRegistersIntoBuffer(requestPDU.data + 5, values);

    // PDU size: 6 bytes + bytes needed for values to write
    requestPDUSize = 6 + requestPDU.data[4];

    return sendRequestToServer_(requestPDU, requestPDUSize);
}

//-----------------------------------------------------------------------------

bool
Client::writeSingleCoil(quint16 regAddress, bool value)
{
    ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Functions::WriteSingleCoil;

    // Coil address
    requestPDU.data[0] = hi(regAddress);
    requestPDU.data[1] = lo(regAddress);

    // 0xFF00 - on, 0x0000 - off
    // All other values will be ignored
    // See: Modbus Protocol Specification v1.1b3, p. 17

    if (value)
        requestPDU.data[2] = 255;
    else
        requestPDU.data[2] = 0;

    requestPDU.data[3] = 0;

    requestPDUSize = 5;

    return sendRequestToServer_(requestPDU, requestPDUSize);
}

//-----------------------------------------------------------------------------

//bool
//Client::writeDouble(quint16 regNo, double value)
//{
//    QVector<quint16> buf(sizeof(value));

////	quint16* ptr = quint16*(&value);
//    memcpy(&value, buf.data(), sizeof(value));

//    return writeHoldingRegisters(regNo, buf);;
//}

//-----------------------------------------------------------------------------

//bool
//Client::writeDWord(quint16 regNo, quint32 value)
//{
//    QVector<quint16> buf(sizeof(value));

//    quint16* ptr = (quint16*)(&value);
//    for (unsigned int i = 0; i < sizeof(value); ++i)
//        buf[i] = ptr[i];

//    return writeHoldingRegisters(regNo, buf);;
//}


//-----------------------------------------------------------------------------

bool
Client::writeSingleRegister(quint16 regAddress, quint16 value)
{
    ProtocolDataUnit requestPDU;
    int requestPDUSize = 0;

    requestPDU.functionCode = Functions::WriteSingleRegister;

    // Register address
    requestPDU.data[0] = hi(regAddress);
    requestPDU.data[1] = lo(regAddress);

    // Register value
    requestPDU.data[2] = hi(value);
    requestPDU.data[3] = lo(value);

    requestPDUSize = 5;

    return sendRequestToServer_(requestPDU, requestPDUSize);
}

//-----------------------------------------------------------------------------

//bool
//Client::writeSingle(quint16 regNo, float value)
//{
//    QVector<quint16> buf(sizeof(value));

//    quint16* ptr = (quint16*)(&value);
//    for (unsigned int i = 0; i < sizeof(value); ++i)
//        buf[i] = ptr[i];

//    return writeHoldingRegisters(regNo, buf);;
//}

//-----------------------------------------------------------------------------

//bool
//Client::writeString(quint16 /*regNo*/, const QString& text)
//{
//    bool result = false;

//    if (!text.isEmpty())
//    {
//        // do some work
//    }

//    return result;
//}

//-----------------------------------------------------------------------------

bool
Client::userDefinedFunction(quint8 function, const QVector<quint8>& data, QVector<quint8>& retData)
{
    ProtocolDataUnit pdu;

    int pduSize = 0;

    pdu.functionCode = function;

    for (quint16 i = 0; (i < data.size()) && (i < PDUDataMaxSize); ++i)
        pdu.data[i] = data[i];

    pduSize = 1 + data.size();

    ProtocolDataUnit replyPdu;
    bool isOk = sendRequestToServer_(pdu, pduSize, &replyPdu);

    if (!isOk)
        return false;
    else
    {
        retData.clear();
        for (int i = 0; i < PDUDataMaxSize; ++i)
            retData.append(pdu.data[i]);

        return true;
    }
}

//-----------------------------------------------------------------------------

bool
Client::userDefinedFunction(quint8 function, quint8 subFunction, const QVector<quint8>& data, QVector<quint8>& retData)
{
    ProtocolDataUnit pdu;

    int pduSize = 0;

    pdu.functionCode = function;
    pdu.data[0] = subFunction;

    for (quint16 i = 0; (i < data.size()) && (i < (PDUDataMaxSize - 1)); ++i)
        pdu.data[i + 1] = data[i];

    pduSize = 2 + data.size();

    ProtocolDataUnit replyPdu;
    bool isOk = sendRequestToServer_(pdu, pduSize, &replyPdu);

    if (!isOk)
        return false;
    else
    {
        retData.clear();
        for (int i = 0; i < PDUDataMaxSize; ++i)
            retData.append(replyPdu.data[i]);

        return true;
    }
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt
