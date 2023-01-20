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

#include "abstract_device.h"

class QIODevice;

namespace modbus4qt
{

//class Device;

//!
//! \brief Abtract modbus server
//!
class Server : public AbstractDevice
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
        //! \brief Add coils into server memory structure and init them with false value
        //! \param startIndex - start index for adding coils
        //! \param endIndex - and index for addign coils
        //!
        //! If some of adding coils with index is already exist skip.
        //!
        void addCoils(quint16 startIndex, quint16 endIndex);

        //!
        //! \brief Add descrete input into server memory structure and init it with false value
        //! \param index - new descerte input index
        //!
        //! If descrete input with index is already exist do nothing.
        //!
        void addDescreteInput(quint16 index);

        //!
        //! \brief Add descrete inputs into server memory structure and init them with false value
        //! \param startIndex - start index for adding descrete inputs
        //! \param endIndex - and index for addign descrete inputs
        //!
        //! If some of adding descrete input with index is already exist skip.
        //!
        void addDescreteInputs(quint16 startIndex, quint16 endIndex);

        //!
        //! \brief Add holding register into server memory structure and init it with zero
        //! \param index - new holding register index
        //!
        //! If holding register with index is already exist do nothing.
        //!
        void addHoldingRegister(quint16 index);

        //!
        //! \brief Add holding registers into server memory structure and init them with zero
        //! \param startIndex - start index for adding holding registers
        //! \param endIndex - and index for addign holding registers
        //!
        //! If some of adding holding registers with index is already exist skip.
        //!
        void addHoldingRegisters(quint16 startIndex, quint16 endIndex);

        //!
        //! \brief Add input register into server memory structure and init it with zero
        //! \param index - new input register index
        //!
        //! If input register with index is already exist do nothing.
        //!
        void addInputRegister(quint16 index);

        //!
        //! \brief Add input registers into server memory structure and init them with zero
        //! \param startIndex - start index for adding input registers
        //! \param endIndex - and index for addign input registers
        //!
        //! If some of adding input registers with index is already exist skip.
        //!
        void addInputRegisters(quint16 startIndex, quint16 endIndex);

//        virtual void readCoilData() = 0;

    public slots :

        //!
        //! \brief process incoming modbus request
        //!
        void processIncomingRequest();

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

        //!
        //! \brief Values of coils in server memory
        //!
        QMap<quint16, bool> coils_;

        //!
        //! \brief Values of descrete inputs in server memory
        //!
        QMap<quint16, bool> descreteInputs_;

        //!
        //! \brief Values of holding registers in server memory
        //!
        QMap<quint16, quint16> holdingRegisters_;

        //!
        //! \brief Values of input registers in server memory
        //!
        QMap<quint16, quint16> inputRegisters_;
};

} // namespace modbus4qt

#endif // SERVER_H

//-----------------------------------------------------------------------------
// EOF
