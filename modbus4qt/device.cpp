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

#include <QByteArray>
#include <QObject>
#include <QVector>

#include "memory_utils.h"

#include "device.h"

namespace modbus4qt
{


//-----------------------------------------------------------------------------

Device::Device()
{

}

//-----------------------------------------------------------------------------

QVector<bool>
Device::getCoilsFromBuffer(const QByteArray& buffer, uint16_t regQty)
{
    QVector<bool> coils(regQty);
    coils.fill(false);

    /*
    The coils in the response message are packed as one coil per bit of the data field. Status is
    indicated as 1=ON and 0=OFF. The LSB of the first data byte contains the output addressed
    in the query. The other coils follow toward the high order end of this byte, and from low order
    to high order in subsequent bytes
    */

    // For every byte from buffer
    //
    for(int i = 0; i < buffer.size(); ++i)
    {
        uint8_t bitMask = 1;

        bool breakFlag = false;

        //  For every bit from bytes
        //
        for (int j = 0; j < 8; ++j)
        {
            int coilNum = i * 8 + j;

            if (coilNum == regQty)
            {
                breakFlag = true;

                break;
            }

            if (buffer[i] & bitMask)
            {
                coils[coilNum] = 1;
            }
            else
            {
                coils[coilNum] = 0;
            }

            bitMask = bitMask << 1;
        }

        if (breakFlag)
        {
            break;
        }
    }

    return coils;
}


//-----------------------------------------------------------------------------

Device::ProtocolDataUnit
Device::getPduFromAdu_(const QByteArray &adu)
{
    ProtocolDataUnit pdu;

    ErrorCodes errorCode;

    if (!extractPDU(adu, pdu, errorCode))
    {
        switch (errorCode)
        {
            case TOO_SHORT_ADU :
                errorMessage_ = QObject::tr("Application data unit recieved less then 5 bytes!");
            break;

            case CRC_MISMATCH :
                errorMessage_ = QObject::tr("CRC mismatch!");
            break;
            default: ; // default is empty
        }
    }

    return pdu;
}

//-----------------------------------------------------------------------------

QVector<uint16_t>
Device::getRegistersFromBuffer(const QByteArray& buffer, uint16_t regQty)
{
    QVector<uint16_t> regValues(regQty);
    regValues.fill(0);

    const char* charBuffer = buffer.constData();

    const uint16_t* ptr = (const quint16*)charBuffer;

    for (uint16_t i = 0; i < regQty; ++i)
    {
        regValues[i] = net2host(*ptr);
        ++ptr;
    }

    return regValues;
}

//-----------------------------------------------------------------------------

QString
Device::lastErrorMessage()
{
    QString result = errorMessage_;
    errorMessage_ = "";

    return result;

}

//-----------------------------------------------------------------------------

void
Device::putCoilsIntoBuffer(const QVector<bool>& data, uint8_t* buffer)
{
    uint8_t bitMask = 1;
    uint8_t* ptr = buffer;

    int regQty = data.size();

    // Clear the buffer
    //
    for (int i = 0; i < (regQty + 7) / 8; ++i)
    {
        ptr[i] = 0x00;
    }

    // And fill values
    //
    for(int i = 0; i < regQty; ++i)
    {
        if (data[i] != 0)
        {
            *ptr |= bitMask;
        }
        else
        {
            *ptr &= ~bitMask;
        }

        if (bitMask == 0x80)
        {
            bitMask = 1;
            ++ptr;
        }
        else
        {
            bitMask = bitMask << 1;
        }

        if (i > MAX_COILS_FOR_WRITE)
        {
            break;
        }
    }
}

//-----------------------------------------------------------------------------

void
Device::putRegistersIntoBuffer(const QVector<quint16>& data, uint8_t* buffer)
{
    uint16_t* ptr = (uint16_t*)buffer;

    for (uint16_t i = 0; i < data.size(); ++i)
    {
        *ptr = host2net(data[i]);
        ++ptr;

        if (i > MAX_COILS_FOR_WRITE)
        {
            break;
        }
    }
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt

//-----------------------------------------------------------------------------
// EOF
