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

#include <QtGlobal>
#include <QDebug>

#include "utils.h"
#include "consts.h"

namespace modbus4qt
{

/* Table of CRC values for high-order byte */
const qint8 table_crc_hi[] = {
    '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41', '\x01', '\xC0',
    '\x80', '\x41', '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41',
    '\x00', '\xC1', '\x81', '\x40', '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0',
    '\x80', '\x41', '\x01', '\xC0', '\x80', '\x41', '\x00', '\xC1', '\x81', '\x40',
    '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41', '\x00', '\xC1',
    '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41', '\x01', '\xC0', '\x80', '\x41',
    '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41', '\x00', '\xC1',
    '\x81', '\x40', '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41',
    '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41', '\x01', '\xC0',
    '\x80', '\x41', '\x00', '\xC1', '\x81', '\x40', '\x00', '\xC1', '\x81', '\x40',
    '\x01', '\xC0', '\x80', '\x41', '\x01', '\xC0', '\x80', '\x41', '\x00', '\xC1',
    '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41', '\x00', '\xC1', '\x81', '\x40',
    '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41', '\x01', '\xC0',
    '\x80', '\x41', '\x00', '\xC1', '\x81', '\x40', '\x00', '\xC1', '\x81', '\x40',
    '\x01', '\xC0', '\x80', '\x41', '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0',
    '\x80', '\x41', '\x01', '\xC0', '\x80', '\x41', '\x00', '\xC1', '\x81', '\x40',
    '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41', '\x01', '\xC0',
    '\x80', '\x41', '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41',
    '\x00', '\xC1', '\x81', '\x40', '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0',
    '\x80', '\x41', '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41',
    '\x01', '\xC0', '\x80', '\x41', '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0',
    '\x80', '\x41', '\x00', '\xC1', '\x81', '\x40', '\x00', '\xC1', '\x81', '\x40',
    '\x01', '\xC0', '\x80', '\x41', '\x01', '\xC0', '\x80', '\x41', '\x00', '\xC1',
    '\x81', '\x40', '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41',
    '\x00', '\xC1', '\x81', '\x40', '\x01', '\xC0', '\x80', '\x41', '\x01', '\xC0',
    '\x80', '\x41', '\x00', '\xC1', '\x81', '\x40'
};

/* Table of CRC values for low-order byte */
const qint8 table_crc_lo[] = {
    '\x00', '\xC0', '\xC1', '\x01', '\xC3', '\x03', '\x02', '\xC2', '\xC6', '\x06',
    '\x07', '\xC7', '\x05', '\xC5', '\xC4', '\x04', '\xCC', '\x0C', '\x0D', '\xCD',
    '\x0F', '\xCF', '\xCE', '\x0E', '\x0A', '\xCA', '\xCB', '\x0B', '\xC9', '\x09',
    '\x08', '\xC8', '\xD8', '\x18', '\x19', '\xD9', '\x1B', '\xDB', '\xDA', '\x1A',
    '\x1E', '\xDE', '\xDF', '\x1F', '\xDD', '\x1D', '\x1C', '\xDC', '\x14', '\xD4',
    '\xD5', '\x15', '\xD7', '\x17', '\x16', '\xD6', '\xD2', '\x12', '\x13', '\xD3',
    '\x11', '\xD1', '\xD0', '\x10', '\xF0', '\x30', '\x31', '\xF1', '\x33', '\xF3',
    '\xF2', '\x32', '\x36', '\xF6', '\xF7', '\x37', '\xF5', '\x35', '\x34', '\xF4',
    '\x3C', '\xFC', '\xFD', '\x3D', '\xFF', '\x3F', '\x3E', '\xFE', '\xFA', '\x3A',
    '\x3B', '\xFB', '\x39', '\xF9', '\xF8', '\x38', '\x28', '\xE8', '\xE9', '\x29',
    '\xEB', '\x2B', '\x2A', '\xEA', '\xEE', '\x2E', '\x2F', '\xEF', '\x2D', '\xED',
    '\xEC', '\x2C', '\xE4', '\x24', '\x25', '\xE5', '\x27', '\xE7', '\xE6', '\x26',
    '\x22', '\xE2', '\xE3', '\x23', '\xE1', '\x21', '\x20', '\xE0', '\xA0', '\x60',
    '\x61', '\xA1', '\x63', '\xA3', '\xA2', '\x62', '\x66', '\xA6', '\xA7', '\x67',
    '\xA5', '\x65', '\x64', '\xA4', '\x6C', '\xAC', '\xAD', '\x6D', '\xAF', '\x6F',
    '\x6E', '\xAE', '\xAA', '\x6A', '\x6B', '\xAB', '\x69', '\xA9', '\xA8', '\x68',
    '\x78', '\xB8', '\xB9', '\x79', '\xBB', '\x7B', '\x7A', '\xBA', '\xBE', '\x7E',
    '\x7F', '\xBF', '\x7D', '\xBD', '\xBC', '\x7C', '\xB4', '\x74', '\x75', '\xB5',
    '\x77', '\xB7', '\xB6', '\x76', '\x72', '\xB2', '\xB3', '\x73', '\xB1', '\x71',
    '\x70', '\xB0', '\x50', '\x90', '\x91', '\x51', '\x93', '\x53', '\x52', '\x92',
    '\x96', '\x56', '\x57', '\x97', '\x55', '\x95', '\x94', '\x54', '\x9C', '\x5C',
    '\x5D', '\x9D', '\x5F', '\x9F', '\x9E', '\x5E', '\x5A', '\x9A', '\x9B', '\x5B',
    '\x99', '\x59', '\x58', '\x98', '\x88', '\x48', '\x49', '\x89', '\x4B', '\x8B',
    '\x8A', '\x4A', '\x4E', '\x8E', '\x8F', '\x4F', '\x8D', '\x4D', '\x4C', '\x8C',
    '\x44', '\x84', '\x85', '\x45', '\x87', '\x47', '\x46', '\x86', '\x82', '\x42',
    '\x43', '\x83', '\x41', '\x81', '\x80', '\x40'
};

//-----------------------------------------------------------------------------

quint16
crc16(const QByteArray& buf)
{
    // The code of this function was taken from libmodbus
    // Этот код взят из libmodbus

    quint16 buffer_length = buf.size();
    quint8* buffer = (quint8*)buf.constData();

    quint8 crc_hi = 0xFF; /* high CRC byte initialized */
    quint8 crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--) {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}

//-----------------------------------------------------------------------------

QVector<bool>
getCoilsFromBuffer(const QByteArray& buffer, quint16 regQty)
{
    QVector<bool> coils(regQty);
    coils.fill(false);

    /*
    The coils in the response message are packed as one coil per bit of the data field. Status is
    indicated as 1= ON and 0= OFF. The LSB of the first data byte contains the output addressed
    in the query. The other coils follow toward the high order end of this byte, and from low order
    to high order in subsequent bytes
    */

    // For every byte from buffer
    for(int i = 0; i < buffer.size(); ++i)
    {
        quint8 bitMask = 1;
        bool breakFlag = false;

        //  For every bit from bytes
        for (int j = 0; j < 8; ++j)
        {
            int coilNum = i * 8 + j;

            if (coilNum == regQty)
            {
                breakFlag = true;
                break;
            }

            if (buffer[i] & bitMask)
                coils[coilNum] = 1;
            else
                coils[coilNum] = 0;

            bitMask = bitMask << 1;
        }

        if (breakFlag) break;
    }

    return coils;
}

//-----------------------------------------------------------------------------

QVector<quint16>
getRegistersFromBuffer(const QByteArray& buffer, quint16 regQty)
{
    QVector<quint16> regValues(regQty);
    regValues.fill(0);

    const char* charBuffer = buffer.constData();

    const quint16* ptr = (const quint16*)charBuffer;

    for (quint16 i = 0; i < regQty; ++i)
    {
        regValues[i] = net2host(*ptr);
        ++ptr;
    }

    return regValues;
}

//-----------------------------------------------------------------------------

void
putCoilsIntoBuffer(quint8* buffer, const QVector<bool>& values)
{
    quint8 bitMask = 1;
    quint8* ptr = buffer;

    int regQty = values.size();

    // Clear the buffer
    for (int i = 0; i < (regQty + 7) / 8; ++i)
        ptr[i] = 0x00;

    // And fill values
    for(int i = 0; i < regQty; ++i)
    {
        if (values[i] != 0)
            *ptr |= bitMask;
        else
            *ptr &= ~bitMask;

        if (bitMask == 0x80)
        {
            bitMask = 1;
            ++ptr;
        }
        else
            bitMask = bitMask << 1;

        if (i > MaxCoilsForWrite) break;
    }
}


//-----------------------------------------------------------------------------

void
putRegistersIntoBuffer(quint8* buffer, const QVector<quint16>& data)
{
    quint16* ptr = (quint16*)buffer;

    for (quint16 i = 0; i < data.size(); ++i)
    {
        *ptr = host2net(data[i]);
        ++ptr;

        if (i > MaxRegistersForWrite) break;
    }
}

} // namespace modbus


