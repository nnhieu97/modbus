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
#include "consts.h"
#include "types.h"

class QIODevice;

namespace modbus4qt
{

//class Device;

//!
//! \brief Abtract modbus server
//!
class Server : public QObject
{
    Q_OBJECT

    public:

        //!
        //! \brief Default constructor
        //! \param parent - parent object
        //!
        explicit Server(QObject *parent = 0);

        //!
        //! \brief Add coil into server memory structure and init it with false value
        //! \param index - new coil index
        //!
        //! If coil with index is already exist do nothing.
        //!
        void addCoil(quint16 index);

        //!
        //! \brief Add coil into server memory structure and init them with false value
        //! \param startIndex - start index for adding coils
        //! \param endIndex - and index for addign coils
        //!
        //! If some of adding coils with index is already exist skip.
        //!
        void addCoils(quint16 startIndex, quint16 endIndex);

//        void addDescreteInput();

//        void addInputRegister();

//        void addHoldingRegister();

//        virtual void readCoilData() = 0;

    public slots :

        //!
        //! \brief process incoming modbus request
        //!
        void processIncomingRequest();

    signals:

        //!
        //! \brief Signal for debuggin info
        //! \param msg - Debug message
        //!
        void debugMessage(const QString& msg);

        //!
        //! \brief Signal for informing about error occured
        //! \param msg - Message with error description
        //!
        void errorMessage(const QString& msg);

        //!
        //! \brief Signal for informing about other events
        //! \param msg - Message with event description
        //!
        void infoMessage(const QString& msg);

    protected:

        virtual ProtocolDataUnit processADU_(const QByteArray& buf) = 0;

    private:

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
        //! \brief Identifier number of server
        //!
        quint8 unitID_;

        // indexes of modbus data...

        //!
        //! \brief Set of coils indexes in server memory
        //!
        QSet<quint16> coilsIndexes_;

        //!
        //! \brief Values of coils in server memory
        //!
        QMap<quint16, bool> coilsData_;
};

} // namespace modbus4qt

#endif // SERVER_H

//-----------------------------------------------------------------------------
// EOF
