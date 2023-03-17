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

#ifndef MODBUS_TCP_CLIENT_H
#define MODBUS_TCP_CLIENT_H

#include <QHostAddress>

#include "client.h"
#include "tcp_device.h"


namespace modbus4qt
{

//!
//! \brief The MODBUS/TCP client class
//!
class TcpClient : public Client, public TCPDevice
{
    Q_OBJECT

    public:

        //!
        //! \brief TcpClient - default constructor
        //! \param parent - Qt parent object
        //!
        explicit TcpClient(QObject* parent = nullptr);

        //!
        //! \brief Connect to server
        //! \param timeout - time to wait connection
        //! \return true in success; false otherwise
        //!
        bool connectToServer(int timeout = DEFAULT_TIMEOUT);

        //!
        //! \brief Disconnect from server
        //!
        void disconnectFromServer();

        //!
        //! \brief Reterun value for auto connect mode
        //! \return true if auto connest is on; flase otherwise
        //!
        bool autoConnect() const;

        //!
        //! \brief Return state of connection with server
        //! \return true if connected; false otherwise
        //!
        virtual bool isConnected() const;

        //!
        //! \brief Return server address
        //! \return Server address
        //!
        QHostAddress getServerAddress() const;

        //!
        //! \brief Set auto connect mode
        //! \param autoConnect - new value for mode
        //!
        void setAutoConnect(bool autoConnect = true);

        //! Устанавливает адрес и порт сервера для подключения
        /**
            Если ранее было установлено соединение с другим сервером, то соединение будет закрыто.
        */

        //!
        //! \brief Set server address and port
        //! \param serverAddress - new server address
        //! \param port - new server port
        //!
        //! If connection established it'll be clesed
        //!
        void setServerAddress(const QHostAddress& serverAddress, const int port = DEFAULT_TCP_PORT);

        //!
        //! \brief Set server port
        //! \param port - new server port
        //!
        //! If connection established it'll be clesed
        //!
        void setServerPort(const int& port);

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


    protected : // fields

        //!
        //! \brief Flag showing if we should use auto connect
        //!
        bool autoConnect_;

        //!
        //! \brief Connection timeout
        //!
        int connectTimeOut_;

        //!
        //! \brief Server address
        //!
        QHostAddress serverAddress_;

    protected : // methods

        //!
        //! \brief Calculate and return next transaction ID
        //! \return Next transaction ID
        //!
        //! Internal value for transaction ID also will be set.
        //!
        uint16_t nextTransactionID_();

        //!
        //! \brief Read response from server, extract PDU from ADU and return it
        //! \param pdu - var to response pdu
        //! \return true in success; false otherwise
        //!
        virtual bool readResponseFromServer_(ProtocolDataUnit& pdu) override;

        //!
        //! \brief Prepare ADU and send request to modbus server
        //! \param requestPDU - PDU for request
        //! \param requestPDUSize - PDU size
        //! \return true in success; false otherwise
        //!
        virtual bool sendRequestToServer_(const ProtocolDataUnit& requestPDU,  int requestPDUSize) override;

        //!
        //! \brief Send data to modbus serve
        //! \param request - data to be sent
        //! \return true in success; false otherwise
        //!
        virtual bool sendDataToServer_(const QByteArray& request) override;


    private slots:

        //!
        //! \brief Process unitDebugMessage_() signal, add server address to message and emit debugMessage()
        //! \param unitID - unit ID
        //! \param msg - message
        //!
        void onUnitDebugMessage_(const QString& msg);

        //!
        //! \brief Process unitErrorMessage_() signal, add server address to message and emit errorMessage()
        //! \param unitID - unit ID
        //! \param msg - message
        //!
        void onUnitErrorMessage_(const QString& msg);

        //!
        //! \brief Process unitInfoMessage_() signal, add server address to message and emit infoMessage()
        //! \param unitID - unit ID
        //! \param msg - message
        //!
        void onUnitInfoMessage_(const QString& msg);

};

} // namespace modbus

#endif // MODBUS_TCP_CLIENT_H
