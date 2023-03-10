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

#ifndef MODBUS4QT_ABSTRACT_DEVICE_H
#define MODBUS4QT_ABSTRACT_DEVICE_H

#include <QObject>

#include "global.h"
#include "consts.h"
#include "types.h"

namespace modbus4qt
{

//!
//! \brief The AbstractDevice class is a base class for any device supporting modbus protocol.
//!
//! In our case abstract device is not an external device.
//! It is a PC or PLC where application runs.
//! It could be client or server.
//!
//! If you connecting to external device supporting modbus protocol you should
//! use client to make connection.
//!
//! If we want represent PC or PLC as modbus device we should use server.
//!
class AbstractDevice : public QObject
{
    Q_OBJECT
    public:

        //!
        //! \brief Return default read/write timeout, 3000 ms.
        //!
        static int defaultTimeout();

        //!
        //! \brief Return max size of protocol data unit, bytes (253).
        //!
        static int PDUMaxSize();

        //!
        //! \brief Return max size of data in protocol data unit, bytes (252).
        //!
        static int PDUDataMaxSize();

        //!
        //! \brief Return default port for MODBUS/TCP (502).
        //!
        static int defaultTCPPort();

        //!
        //! \brief Return unit ID which will be ignored (for MODBUS/TCP only) - 255.
        //!
        static quint8 ignoreUnitID();

        //!
        //! \brief Return unit ID for broadcast message (for MODBUS/TCP only) - 0.
        //!
        static quint8 broadcastUnitID();

        //!
        //! \brief Return maximum coils quantity for reading (2000).
        //!
        //! See: Modbus Protocol Specification v1.1b3, p. 11
        //!
        static int maxCoilsForRead();

        //!
        //! \brief Return maximum coils quantity for writing (1968).
        //!
        //! See: Modbus Protocol Specification v1.1b3, p. 29
        //!
        static int maxCoilsForWrite();

        //!
        //! \brief Return maximum registers quantity for reading (125)
        //!
        //! See: Modbus Protocol Specification v1.1b3, p. 15
        //!
        static int maxRegistersForRead();

        //!
        //! \brief Return maximum registers quantity for writing (23)
        //!
        //! See: Modbus Protocol Specification v1.1b3, p. 30
        //!
        static int  maxRegistersForWrite();

        //!
        //! \brief The ErrorCodes enum represents error codes for modbus protocol.
        //!
        enum ErrorCodes
        {
            NO_ERROR        = 0,
            TOO_SHORT_ADU   = 1,
            CRC_MISMATCH    = 2

        };
        Q_ENUM(ErrorCodes)

        //!
        //! \brief The Functions enum - modbus function codes
        //!
        //! Commented codes is not implemented in current version
        //!
        enum class Functions:quint8
        {
            READ_COILS                          = 0x01, //! Read data block from coils table
            READ_DESCRETE_INPUTS                = 0x02, //! Read data block from discrete inputs table
            READ_HOLDING_REGISTERS              = 0x03, //! Read data block from holding registers table
            READ_INPUT_REGISTERS                = 0x04, //! Read data block from input registers table
            WRITE_SINGLE_COIL                   = 0x05, //! Write single value to coils table
            WRITE_SINGLE_REGISTER               = 0x06, //! Write single value to holding registers table
            //ReadExceptionStatus               = 0x07,
            //Diagnostics                       = 0x08,
            //GetCommEventCounter               = 0x0,
            //GetCommEventLog                   = 0x0C,
            WRITE_MULTIPLE_COILS                = 0x0F, //! Write data block to coils table
            WRITE_MULTIPLE_REGISTERS            = 0x10, //! Write data block from holding registers table
            //ReportServerID                    = 0x11,
            //ReadFileRecord                    = 0x14,
            //WriteFileRecord                   = 0x15,
            //MaskWriteRegister                 = 0x16
            //ReadWritMultipleeRegisters        = 0x17
            //ReadFIFOQueue                     = 0x18
            //EncapsulatedInterfaceTransport    = 0x2B
        };
        Q_ENUM(Functions)

        //!
        //! \brief The Exception enum - modbus error codes
        //!
        enum class Exceptions:quint8
        {
            OK                                          = 0x00, //! No errors
            ILLEGAL_FUNCTION                            = 0x01, //! Illegal function
            ILLEGAL_DATA_ADDRESS                        = 0x02, //! Illegal data address
            ILLEGAL_DATA_VALUE                          = 0x03, //! Illegal data value
            SERVER_DEVICE_FAILURE                       = 0x04, //! Server device falilure
            ACKNOWLEDGE                                 = 0x05, //! Acknowledge
            SERVER_DEVICE_BUSY                          = 0x06, //! Server device busy
            MEMORY_PARITY_ERROR                         = 0x08, //! Memory parity error
            GATEWAY_PATH_NOT_AVAILABLE                  = 0x0A, //! Gateway path not available
            GATEWAY_TARGET_DEVICE_FAILED_TO_RESPONSE    = 0x0B, //! Gateway target device failed to response
        };
        Q_ENUM(Exceptions)

        /**
         * Constants which defines the format of a modbus frame. The example is
         * shown for a Modbus RTU/ASCII frame. Note that the Modbus PDU is not
         * dependent on the underlying transport.
         *
         * <code>
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
         * </code>
         */

        /*
         *
         * <------------------------ MODBUS TCP/IP ADU(1) ------------------------->
         *              <----------- MODBUS PDU (1') ---------------->
         *  +-----------+---------------+------------------------------------------+
         *  | TID | PID | Length | UID  |Code | Data                               |
         *  +-----------+---------------+------------------------------------------+
         *  |     |     |        |      |
         * (2)   (3)   (4)      (5)    (6)
         *
         * (2)  ... MB_TCP_TID          = 0 (Transaction Identifier - 2 Byte)
         * (3)  ... MB_TCP_PID          = 2 (Protocol Identifier - 2 Byte)
         * (4)  ... MB_TCP_LEN          = 4 (Number of bytes - 2 Byte)
         * (5)  ... MB_TCP_UID          = 6 (Unit Identifier - 1 Byte)
         * (6)  ... MB_TCP_FUNC         = 7 (Modbus Function Code)
         *
         * (1)  ... Modbus TCP/IP Application Data Unit
         * (1') ... Modbus Protocol Data Unit
         */

        #pragma pack(push, 1)

        //!
        //! \brief Protocol Data Unit is used in all types of communication lines.
        //!
        //! See: Modbus Protocol Specification v1.1b3, page 5
        //!
        struct ProtocolDataUnit
        {
            //!
            //! \brief modbus function code
            //!
            Functions functionCode;

            //!
            //! \brief Data to send
            //!
            uint8_t data[PDUDataMaxSize()];

            //!
            //! \brief Default constructor
            //!
            //! Fill data with zeros.
            //!
            ProtocolDataUnit() // : functionCode(0)
            {
                std::fill(data, data + PDUDataMaxSize(), 0);
            }

            //!
            //! \brief Copying constructor
            //! \param rhv - right hand value
            //!
            //! Copy data array from rhv.
            //!
            ProtocolDataUnit(const ProtocolDataUnit& rhv)
            {
                functionCode = rhv.functionCode;
                std::copy(rhv.data, rhv.data + PDUDataMaxSize(), data);
            }

            //!
            //! \brief Copy data array from rhv
            //! \param rhv - right hand value
            //! \return Copied entity
            //!
            ProtocolDataUnit& operator=(const ProtocolDataUnit& rhv)
            {
                functionCode = rhv.functionCode;
                std::copy(rhv.data, rhv.data + PDUDataMaxSize(), data);

                return *this;
            }
        };

        //!
        //! \brief Application data unit for MODBUS over serial line
        //!
        struct RTUApplicationDataUnit
        {
            //!
            //! \brief Address of device
            //!
            //! Should be from 1 to 247. Range 248-255 is reserved by protocol specification.
            //!
            uint8_t unitId;

            //!
            //! \brief Protocol data unit
            //!
            //! \sa ProtocolDataUnit
            //!
            ProtocolDataUnit pdu;

            //!
            //! \brief Error checking field is the result of a "Redundancy Checking"
            //!
            //! CRC calculation is performed on the message content.
            //!
            uint16_t crc;
        };

        /**
         * @brief The TCPApplicationDataUnit struct
         */
        struct TCPApplicationDataUnit
        {
            //! @todo Это пока не важно, надо исправить в соответствие со специйикацией
            quint8 unitId;
            ProtocolDataUnit pdu;
            quint16 crc;
        };

        #pragma pack(pop)

        //!
        //! \brief AbstractDevice - default constructor.
        //! \param parent - Qt parent object
        //!
        explicit AbstractDevice(QObject* parent = nullptr);

        //!
        //! \brief Process Modbus-RTU application data unit and return protocol data unit
        //! \param buf - array with data readed, should contain ADU record
        //! \return Protocol data unit
        //!
        static bool preparePDUForRTU(const QByteArray& buf, ProtocolDataUnit& pdu, ErrorCodes& errorCode);

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

    protected:

        /**
         * @brief
         * Forms protocol data unit
         *
         * Function should be realized in descendent class since format of application
         * data unit depends from physical commincation line used.
         *
         * @param
         * pdu - Protocol Data Unit
         *
         * @param
         * pduSize - size of protocol data unit (in bytes)
         *
         * @return
         * Application Data Unit formed
         *
         * @sa ProtocolDataUnit, RTUApplicationDataUnit
         *
         */
        virtual QByteArray prepareADU_(const ProtocolDataUnit& pdu, int pduSize) = 0;

        /**
         * @brief
         * Process application data unit recieved from server and returns protocol data unit
         *
         * Function should be realized in descendent class since format of application
         * data unit depends from physical commincation line used (RTU or TCP).
         *
         * @param
         * buf - data recieved from server
         *
         * @return
         * Protocol data unit
         *
         * @sa ProtocolDataUnit, RTUApplicationDataUnit
         *
         */
        virtual ProtocolDataUnit processADU_(const QByteArray& buf) = 0;

        /**
         * @brief
         * Read response from slave device
         *
         * @return
         * Readed data array
         *
         * As timeout processing is differ for RTU and TCP we need to implement
         * method in descendance.
         */
        virtual QByteArray readResponse_() = 0;
};

} // namespace modbus4qt

#endif // MODBUS4QT_ABSTRACT_DEVICE_H
