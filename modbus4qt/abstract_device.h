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
//! In our case abstract device is not external device. It is a device
//! presenting in PC where application runs. It sould be client or server.
//!
//! If you connecting to external device supporting modbus protocol you should
//! use client to make connection.
//!
class AbstractDevice : public QObject
{
        Q_OBJECT
    public:

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
        //! \brief AbstractDevice - default constructor
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
