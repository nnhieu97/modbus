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
#include <QMutex>
#include <QWaitCondition>

#include "abstract_device.h"

namespace modbus4qt
{

// Table of CRC values for high-order byte
//
const int8_t table_crc_hi[] = {
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

// Table of CRC values for low-order byte
//
const int8_t table_crc_lo[] = {
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

AbstractDevice::AbstractDevice(QObject *parent)
    : QObject(parent)
{

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
    adu.pdu.functionCode = tempBuf[1];

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

uint16_t
AbstractDevice::crc16(const QByteArray& buf)
{
    // The code of this function was taken from libmodbus

    uint16_t buffer_length = buf.size();
    uint8_t* buffer = (quint8*)buf.constData();

    uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    // pass through message buffer
    //
    while (buffer_length--)
    {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}

//-----------------------------------------------------------------------------

QVector<bool>
AbstractDevice::getCoilsFromBuffer(const QByteArray& buffer, quint16 regQty)
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

QVector<quint16>
AbstractDevice::getRegistersFromBuffer(const QByteArray& buffer, quint16 regQty)
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

quint8
AbstractDevice::hi(quint16 word)
{
    quint8* bytes = (quint8*)&word;

    quint16 test = 1; // * 0x0001 *
    if (*((quint8*)&test) == 0)
    {
        return bytes[0];	// big endian (network/motorolla)
    }
    else
    {
        return bytes[1];	// little endian (intel)
    }
}

//-----------------------------------------------------------------------------

quint16
AbstractDevice::host2net(quint16 word)
{
    quint16 test = 1; // * 0x0001 *
    if (*((quint8*)&test) == 0)
    {
        return word;		// big endian (network/motorolla)
    }
    else
    {
        return swap(word);	// little endian (intel)
    }
}

//-----------------------------------------------------------------------------

quint8
AbstractDevice::lo(quint16 word)
{
    quint8* bytes = (quint8*)&word;

    quint16 test = 1; // * 0x0001 *
    if (*((quint8*)&test) == 0)
    {
        return bytes[1];	// big endian (network/motorolla)
    }
    else
    {
        return bytes[0];	// little endian (intel)
    }
}

//-----------------------------------------------------------------------------

quint16
AbstractDevice::net2host(quint16 word)
{
    quint16 test = 1; // * 0x0001 *
    if (*((quint8*)&test) == 0)
    {
        return word;		// big endian (network/motorolla)
    }
    else
    {
        return swap(word);	// little endian (intel)
    }
}

//-----------------------------------------------------------------------------

void
AbstractDevice::putCoilsIntoBuffer(quint8* buffer, const QVector<bool>& values)
{
    quint8 bitMask = 1;
    quint8* ptr = buffer;

    int regQty = values.size();

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
        if (values[i] != 0)
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
AbstractDevice::putRegistersIntoBuffer(quint8* buffer, const QVector<quint16>& data)
{
    quint16* ptr = (quint16*)buffer;

    for (quint16 i = 0; i < data.size(); ++i)
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

quint16
AbstractDevice::swap(quint16 word)
{
    quint16 result = lo(word);
    result = result << 8;
    result += hi(word);

    return result;
}

//-----------------------------------------------------------------------------

void
AbstractDevice::wait(int time)
{
    QMutex mutex;
    mutex.lock();

    QWaitCondition pause;
    pause.wait(&mutex, time);
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt

//-----------------------------------------------------------------------------
// EOF
