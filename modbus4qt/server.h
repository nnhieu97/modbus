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

#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QMap>
#include <QSet>

#include "global.h"

#include "device.h"

class QIODevice;

namespace modbus4qt
{

class ServerInternalData;

//!
//! \brief Abtract modbus server
//!
class Server : public QObject
{
    public:

        //!
        //! \brief Default constructor
        //! \param parent - parent object
        //!
        explicit Server(QObject* parent = nullptr);

        explicit Server(ServerInternalData* internalData, QObject* parent = nullptr);

        void setInternalData(ServerInternalData* internalData);

        int readTimeout() const;

        void setReadTimeout(int readTimeout);

        int writeTimeout() const;

        void setWriteTimeout(int writeTimeout);

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

    protected :

        virtual bool checkReciever_(const QByteArray& adu) = 0;

        bool modbusServerTransaction(const Device::ProtocolDataUnit& requestPDU, int pduSize);

        virtual bool processClientRequest_(const QByteArray& requestAdu, const Device::ProtocolDataUnit& requestPdu, int pduSize) = 0;

        //!
        //! \brief Send data to modbus client
        //! \param request - data to be sent
        //! \return true in success; false otherwise
        //!
        virtual bool sendDataToClient_(const QByteArray& response);

    protected slots :

        //!
        //! \brief Read data from client, if we a proper reciever then do server modbus transaction
        //!
        virtual void readClientRequest_();

    protected :

        //!
        //! \brief Read device to read data from
        //!
        //! User can use it if we need to make modbus interface to equipment connected to PC.
        //! PC will be modbus server in this case.
        //!
        //! \todo I do not know if we really need it here! 2022-03-13
        //!
        //Device* device_;

        //!
        //! \brief IO device for data exchange with client (master)
        //!
        QIODevice* ioDevice_;

        //!
        //! \brief Maximum allowed timeout for reading data
        //!
        int readTimeout_;

        //!
        //! \brief Maximum allowed timeout for sending data
        //!
        int writeTimeout_;

        //!
        //! \brief Server internal data
        //!
        ServerInternalData* internalData_ = nullptr;
};

} // namespace modbus4qt

#endif // SERVER_H

//-----------------------------------------------------------------------------
// EOF
