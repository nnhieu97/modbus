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

#ifndef RTU_SERVER_H
#define RTU_SERVER_H

#include "server.h"
#include "rtu_device.h"

namespace modbus4qt
{

class RtuServer : public Server, public RTUDevice
{
    Q_OBJECT
    public:
        explicit RtuServer(const QString& portName,
                           QSerialPort::BaudRate baudRate = QSerialPort::Baud9600,
                           QSerialPort::DataBits dataBits = QSerialPort::Data8,
                           QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                           QSerialPort::Parity parity = QSerialPort::EvenParity,
                           QObject *parent = nullptr);
        //!
        //! \brief RtuClient - Constructor, short version.
        //! \param portName - port name for data exchange
        //! \param baudRate - baud rate
        //! \param dataBits - quantity of data bits
        //! \param stopBits - quantity of stop bits
        //! \param parent - Qt parent object
        //!
        explicit RtuServer(const QString& portName,
                           QSerialPort::BaudRate baudRate = QSerialPort::Baud9600,
                           QSerialPort::DataBits dataBits = QSerialPort::Data8,
                           QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                           QObject *parent = nullptr);

        //!
        //! \brief RtuClient - Constructor, short version.
        //! \param portName - port name for data exchange
        //! \param baudRate - baud rate
        //! \param dataBits - quantity of data bits
        //! \param parent - Qt parent object
        //!
        explicit RtuServer(const QString& portName,
                           QSerialPort::BaudRate baudRate = QSerialPort::Baud9600,
                           QSerialPort::DataBits dataBits = QSerialPort::Data8,
                           QObject *parent = nullptr);

        //!
        //! \brief RtuClient - Constructor, short version.
        //! \param portName - port name for data exchange
        //! \param baudRate - baud rate
        //! \param parent - Qt parent object
        //!
        explicit RtuServer(const QString& portName,
                           QSerialPort::BaudRate baudRate = QSerialPort::Baud9600,
                           QObject *parent = nullptr);

        //!
        //! \brief RtuClient - Constructor, short version.
        //! \param portName - port name for data exchange
        //! \param parent - Qt parent object
        //!
        explicit RtuServer(const QString& portName, QObject *parent = nullptr);

        //!
        //! \brief Open port and set up it with current data exchange settings.
        //! \return true, if port opened succesfullty; false in case of error
        //!
        //! In case of error will set error message.
        bool openPort();

    signals:

        //!
        //! \brief dataReaded signal emits after data reading.
        //!
        void dataReaded();

        //!
        //! \brief Signal for debugging informing
        //! \param unitID - unit ID of server device
        //! \param msg - message with debug information
        //!
        void unitDebugMessage_(quint8 unitID, const QString& msg);

        //!
        //! \brief Signal for informing about errors
        //! \param unitID - unit ID of server device
        //! \param msg - message with error description
        //!
        void unitErrorMessage_(quint8 unitID, const QString& msg);

        //!
        //! \brief Signal for general informing
        //! \param unitID - unit ID of server device
        //! \param msg - message
        //!
        void unitInfoMessage_(quint8 unitID, const QString& msg);

    protected :  // methods

        //!
        //! \brief If port is open set data exchange settings with current values.
        //! \return true in success, false overwise
        //!
        //! If port is not open errorMessage() signal will be emited.
        //!
        virtual bool configurePort_() override;

    private slots:

        //!
        //! \brief Process unitDebugMessage_() signal, add unit ID to message and emit debugMessage()
        //! \param unitID - unit ID
        //! \param msg - message
        //!
        void onUnitDebugMessage_(const QString& msg);

        //!
        //! \brief Process unitErrorMessage_() signal, add unit ID to message and emit errorMessage()
        //! \param unitID - unit ID
        //! \param msg - message
        //!
        void onUnitErrorMessage_(const QString& msg);

        //!
        //! \brief Process unitInfoMessage_() signal, add unit ID to message and emit infoMessage()
        //! \param unitID - unit ID
        //! \param msg - message
        //!
        void onUnitInfoMessage_(const QString& msg);
};

} // namespace modbus4qt

#endif // RTU_SERVER_H
