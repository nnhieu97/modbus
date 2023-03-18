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

#ifndef MODBUS4QT_CLIENT_H
#define MODBUS4QT_CLIENT_H

#include <QObject>

#include "device.h"

class QIODevice;

namespace modbus4qt
{

//!
//! \brief The Client class is abtract modbus client
//!
class Client : public QObject
{
    Q_OBJECT

    public:
        //!
        //! \brief Client - default constructor
        //! \param parent - parent object
        //!
        explicit Client(QObject* parent = nullptr);

        //!
        //! \brief Read single coil value from server
        //! \param regNo - coil address
        //! \param value - var to store result
        //! \return true in success; false otherwise
        //!
        bool readCoil(uint16_t regNo, bool& value);

        //!
        //! \brief Read array of coils values from server
        //! \param regStart - address of first coil
        //! \param regQty - quantity of coils reading
        //! \param values - array for readed values
        //! \return true in success; false otherwise
        //!
        bool readCoils(uint16_t regStart, uint16_t regQty, QVector<bool>& values);

        //!
        //! \brief Read single descrete inputs value from server
        //! \param regNo - coil address
        //! \param value - var to store result
        //! \return true in success; false otherwise
        //!
        bool readDescreteInput(uint16_t regNo, bool& value);

        //!
        //! \brief Read array of descrete inputs values from server
        //! \param regStart - address of first coil
        //! \param regQty - quantity of coils reading
        //! \param values - array for readed values
        //! \return true in success; false otherwise
        //!
        bool readDescreteInputs(uint16_t regStart, uint16_t regQty, QVector<bool>& values);

        //!
        //! \brief Read holding register value from server
        //! \param regNo - holding register address
        //! \param value - var to store result
        //! \return true in success; false otherwise
        //!
        bool readHoldingRegister(uint16_t regNo, quint16& value);

        //!
        //! \brief Read array of holding registers values from server
        //! \param regStart - address of first coil
        //! \param regQty - quantity of coils reading
        //! \param values - array for readed values
        //! \return true in success; false otherwise
        //!
        bool readHoldingRegisters(uint16_t regStart, uint16_t regQty, QVector<uint16_t>& values);

        //!
        //! \brief Read input register value from server
        //! \param regNo - holding register address
        //! \param value - var to store result
        //! \return true in success; false otherwise
        //!
        bool readInputRegister(uint16_t regNo, uint16_t& value);

        //!
        //! \brief Read array of input registers values from server
        //! \param regStart - address of first coil
        //! \param regQty - quantity of coils reading
        //! \param values - array for readed values
        //! \return true in success; false otherwise
        //!
        //!
        bool readInputRegisters(uint16_t regStart, uint16_t regQty, QVector<quint16>& values);

        //!
        //! \brief Return value for reading timeout
        //! \return Value for reading timeout
        //!
        int readTimeout() const;

        //!
        //! \brief Set new value for reading timeout
        //! \param readTimeout - new value for reading timeout, ms
        //!
        void setReadTimeOut(int readTimeout);

        //!
        //! \brief Set new value for writing timeout
        //! \param writeTimeout - new value for writing timeout, ms
        //!
        void setWriteTimeOut(int writeTimeout);

        //!
        //! \brief Send user defined function command to server
        //! \param function - function code
        //! \param data - extra data to be sent
        //! \param retData - data returned from server
        //! \return true in success; false otherwise
        //!
        bool userDefinedFunction(uint8_t function, const QVector<uint8_t>& data, QVector<uint8_t>& retData);

        //!
        //! \brief Send user defined function command to server
        //! \param function - function code
        //! \param subFunction - subfinction code
        //! \param data - extra data to be sent
        //! \param retData - data returned from server
        //! \return true in success; false otherwise
        //!
        bool userDefinedFunction(uint8_t function, uint8_t subFunction, const QVector<uint8_t>& data, QVector<uint8_t>& retData);

        //!
        //! \brief Write coil value to server
        //! \param regNo - address of the coil
        //! \param value - value to write
        //! \return true in success; false otherwise
        //!
        bool writeCoil(uint16_t regNo, bool value);

        //!
        //! \brief Write array of coils values to server
        //! \param regStart - address of first coil
        //! \param values - array for values to write
        //! \return true in success; false otherwise
        //!
        bool writeCoils(uint16_t regStart, const QVector<bool>& values);

        //!
        //! \brief Write holding register value to server
        //! \param regNo - address of the holding register
        //! \param value - value to write
        //! \return true in success; false otherwise
        //!
        bool writeHoldingRegister(uint16_t regAddress, uint16_t value);

        //!
        //! \brief Write array of holding registers value to server
        //! \param regStart - address of first coil
        //! \param values - array for values to write
        //! \return true in success; false otherwise
        //!
        bool writeHoldingRegisters(uint16_t regStart, const QVector<uint16_t>& values);

        //!
        //! \brief Return value of writing timeout
        //! \return Value of writing timeout
        //!
        int writeTimeout() const;

    signals:

        //!
        //! \brief Signal for debugging informing
        //! \param msg - Debug message
        //!
        void debugMessage(const QString& msg);

        //!
        //! \brief Signal for informing about error occured
        //! \param msg - Message with error description
        //!
        void errorMessage(const QString& msg);

        //!
        //! \brief Signal for general purposes informing
        //! \param msg - message
        //!
        void infoMessage(const QString& msg);

    protected: // fields

        //!
        //! \brief Device for data exchange with server
        //!
        QIODevice* ioDevice_;

        //!
        //! \brief Read timeout
        //!
        int readTimeout_;

        //!
        //! \brief Write timeout
        //!
        int writeTimeout_;

    protected : // methods

        //!
        //! \brief Do modbus transaction: send request PDU to server and read response PDU
        //! \param requestPDU - request PDU
        //! \param requestPDUSize - request PDU size
        //! \param responsePDU - response PDU
        //! \return true in success; false otherwise
        //!
        bool modbusClientTransaction(const Device::ProtocolDataUnit& requestPDU, int requestPDUSize, Device::ProtocolDataUnit& responsePDU);

        //!
        //! \brief Do modbus transaction: send request PDU to server and read response PDU
        //! \param requestPDU - request PDU
        //! \param requestPDUSize - request PDU size
        //! \return true in success; false otherwise
        //!
        //! It is short version to use when we does not need analize response from server
        //!
        bool modbusClientTransaction(const Device::ProtocolDataUnit& requestPDU, int requestPDUSize);

        //!
        //! \brief Read data from server
        //! \param response - array for data readed from server
        //! \return true in success; false otherwise
        //!
        virtual bool readAduFromServer_(QByteArray& response);

        //!
        //! \brief Read response from server, extract PDU from ADU and return it
        //! \param pdu - var to response pdu
        //! \return true in success; false otherwise
        //!
        virtual bool readResponseFromServer_(Device::ProtocolDataUnit& pdu) = 0;

        //!
        //! \brief Send data to modbus server
        //! \param request - data to be sent
        //! \return true in success; false otherwise
        //!
        virtual bool sendDataToServer_(const QByteArray& request);

        //!
        //! \brief Prepare ADU and send request to modbus server
        //! \param requestPDU - PDU for request
        //! \param requestPDUSize - PDU size
        //! \return true in success; false otherwise
        //!
        virtual bool sendRequestToServer_(const Device::ProtocolDataUnit& requestPDU,  int requestPDUSize) = 0;

};

} // namespace modbus4qt

#endif // MODBUS4QT_CLIENT_H
