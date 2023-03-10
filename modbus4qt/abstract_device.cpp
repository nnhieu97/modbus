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

#include <QDebug>
#include <QByteArray>

#include "utils.h"

#include "abstract_device.h"

namespace modbus4qt
{

//-----------------------------------------------------------------------------

AbstractDevice::AbstractDevice(QObject *parent)
    : QObject(parent)
{

}

int AbstractDevice::defaultTimeout()
{
    return 3000;
}

int AbstractDevice::PDUMaxSize()
{
    return 253;
}

int AbstractDevice::PDUDataMaxSize()
{
    return PDUMaxSize() - 1;
}

int AbstractDevice::defaultTCPPort()
{
    return 502;
}

quint8 AbstractDevice::ignoreUnitID()
{
    return 255;
}

quint8 AbstractDevice::broadcastUnitID()
{
    return 0;
}

int AbstractDevice::maxCoilsForRead()
{
    return 2000;
}

int AbstractDevice::maxCoilsForWrite()
{
    return 1968;
}

int AbstractDevice::maxRegistersForRead()
{
    return 125;
}

int AbstractDevice::maxRegistersForWrite()
{
    return 23;
}

//-----------------------------------------------------------------------------

bool
AbstractDevice::preparePDUForRTU(const QByteArray& buf, ProtocolDataUnit& pdu, ErrorCodes& errorCode)
{
    errorCode = NO_ERROR;

    qDebug() << "ADU: " << buf.toHex();
    qDebug() << "ADU size: " << buf.size();

    QByteArray tempBuf(buf);

    // For any case we should check size of recieved data to protect memory
    //
    if (tempBuf.size() > int(sizeof(RTUApplicationDataUnit)))
    {
        tempBuf.resize(sizeof(RTUApplicationDataUnit));
    }

    int tempBufSize = tempBuf.size();

    // Minimum ADU size can be 5 bytes: 1 byte for address, 2 bytes for minimum PDU, 2 bytes for CRC
    //
    if (tempBufSize < 5)
    {
        errorCode = TOO_SHORT_ADU;

        tempBuf.resize(5);
        tempBufSize = 5;
    }

    RTUApplicationDataUnit adu;

    adu.unitId = tempBuf[0];
    adu.pdu.functionCode = static_cast<Exceptions>(tempBuf[1]);

    for (int i = 2; i < tempBufSize - 2; ++i)
    {
        adu.pdu.data[i - 2] = tempBuf[i];
    }

    WordRec aduCRC;
    aduCRC.bytes[0] = tempBuf[tempBufSize - 2];
    aduCRC.bytes[1] = tempBuf[tempBufSize - 1];

    adu.crc = aduCRC.word;

    quint16 recievedCRC = net2host(adu.crc);

    qDebug() << QString("recievied crc: %1").arg(recievedCRC, 4, 16);

    // Calculate CRC for recived packet
    // At first we should delete last 2 bytes, which is the CRC
    //
    tempBuf.resize(tempBuf.size() - 2);
    quint16 calculatedCRC = crc16(tempBuf);

    qDebug() << QString("calculated crc: %1").arg(calculatedCRC, 4, 16);

    // In case of mismatch emit error message
    //
    if (recievedCRC != calculatedCRC)
    {
        errorCode = CRC_MISMATCH;
    }

#ifndef QT_NO_DEBUG
    // If we want to print PDU into log file we should remove 1st byte
    // After that we have pdu in tempBuf as CRC was deleted before
    //
    tempBuf.remove(0, 1);
    qDebug() << "PDU: " << tempBuf.toHex();
    qDebug() << "PDU size: " << tempBuf.size();
#endif

    pdu = adu.pdu;

    if (errorCode == NO_ERROR)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt

//-----------------------------------------------------------------------------
// EOF
