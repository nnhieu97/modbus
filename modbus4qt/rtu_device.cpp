#include <QDebug>

#include "qmutex.h"
#include "qwaitcondition.h"
#include "rtu_device.h"
#include "memory_utils.h"

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

RTUDevice::RTUDevice(const QString& portName,
                     const QSerialPort::BaudRate& baudRate,
                     const QSerialPort::DataBits& dataBits,
                     const QSerialPort::StopBits& stopBits,
                     const QSerialPort::Parity& parity) :
    portName_(portName),
    baudRate_(baudRate),
    dataBits_(dataBits),
    stopBits_(stopBits),
    parity_(parity)
{
    serialPort_ = new QSerialPort;
    serialPort_->setPortName(portName_);

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RTUDevice::~RTUDevice()
{
    serialPort_->close();
    serialPort_->deleteLater();
}

//-----------------------------------------------------------------------------

bool
RTUDevice::configurePort_()
{
    if (!serialPort_->isOpen())
    {
        errorMessage_ =  QObject::tr("Cannot configure port %1. The port is not open!").arg(portName_);
        return false;
    }

    if (!serialPort_->setBaudRate(baudRate_))
    {
        errorMessage_ = serialPort_->errorString();
        return false;
    }

    if (!serialPort_->setDataBits(dataBits_))
    {
        errorMessage_ = serialPort_->errorString();
        return false;
    }

    if (!serialPort_->setStopBits(stopBits_))
    {
        errorMessage_ = serialPort_->errorString();
        return false;
    }

    if (!serialPort_->setParity(parity_))
    {
        errorMessage_ = serialPort_->errorString();
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

uint16_t
RTUDevice::crc16(const QByteArray& buf)
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

bool
RTUDevice::extractPDU(const QByteArray& buf, ProtocolDataUnit& pdu, ErrorCodes& errorCode)
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

bool
RTUDevice::openPort()
{
    if (serialPort_->isOpen())
    {
        serialPort_->close();

    }

    bool result = serialPort_->open(QIODevice::ReadWrite);

    if (!result)
    {
        errorMessage_ = serialPort_->errorString();
        serialPort_->close();
        return false;
    }
    else
    {
        result = configurePort_();

        if (!result)
        {
            serialPort_->close();
        }
    }

    if (!result)
    {
        errorMessage_ = QObject::tr("Error while opening and configuring port!");
    }

    return result;
}

//-----------------------------------------------------------------------------

QString
RTUDevice::portName() const
{
    return portName_;
}

//-----------------------------------------------------------------------------

QByteArray
RTUDevice::prepareADU_(const ProtocolDataUnit &pdu, int pduSize)
{
    /*
     * <------------------------ MODBUS SERIAL LINE PDU (1) ------------------->
     *              <----------- MODBUS PDU (1') ---------------->
     *  +-----------+---------------+----------------------------+-------------+
     *  | Address   | Function Code | Data                       | CRC/LRC     |
     *  +-----------+---------------+----------------------------+-------------+
     *  |           |               |                                   |
     * (2)        (3/2')           (3')                                (4)
     *
     * (1)  ... MB_SER_PDU_SIZE_MAX = 256
     * (2)  ... MB_SER_PDU_ADDR_OFF = 0
     * (3)  ... MB_SER_PDU_PDU_OFF  = 1
     * (4)  ... MB_SER_PDU_SIZE_CRC = 2
     *
     * (1') ... MB_PDU_SIZE_MAX     = 253
     * (2') ... MB_PDU_FUNC_OFF     = 0
     * (3') ... MB_PDU_DATA_OFF     = 1
     */

    QByteArray result;

    result[0] = unitID_;

    result.insert(1, (char*)&pdu, pduSize);

    quint16 crc = host2net(crc16(result));
    result.append((char*)&crc, 2);

    qDebug() << "ADU: " << result.toHex();
    qDebug() << "ADU size: " << result.size();

    return result;
}

//-----------------------------------------------------------------------------

QSerialPort*
RTUDevice::serialPort() const
{
    return serialPort_;
}


//-----------------------------------------------------------------------------

void
RTUDevice::setBaudRate(QSerialPort::BaudRate baudRate)
{
    if (baudRate_ != baudRate)
    {
        baudRate_ = baudRate;
        setSilenceTime_();

        if (serialPort_->isOpen())
        {
            serialPort_->setBaudRate(baudRate_);
        }
    }
}

//-----------------------------------------------------------------------------

void
RTUDevice::setDataBits(QSerialPort::DataBits dataBits)
{
    if (dataBits_ != dataBits)
    {
        dataBits_ = dataBits;

        if (serialPort_->isOpen())
        {
            serialPort_->setDataBits(dataBits_);
        }
    }
}

//-----------------------------------------------------------------------------

void
RTUDevice::setParity(QSerialPort::Parity parity)
{
    if (parity_ != parity)
    {
        parity_ = parity;

        if (serialPort_->isOpen())
        {
            serialPort_->setParity(parity_);
        }
    }
}

//-----------------------------------------------------------------------------

void
RTUDevice::setPortName(const QString& portName)
{
    if (portName != portName_)
    {
        if (serialPort_->isOpen())
        {
            serialPort_->close();
        }

        portName_ = portName;
        serialPort_->setPortName(portName_);
    }
}

//-----------------------------------------------------------------------------

void
RTUDevice::setSilenceTime_()
{
    if (baudRate_ > QSerialPort::Baud19200)
    {
        silenceTime_ = 2; // 2 ms > 1750 microseconds
    }
    else
    {
        // For each byte we should transmit 11 bits
        // We have to calculate time in seconds,
        // power it by 1000 and add 1 for any cases
        //
        silenceTime_ = ((11.0 / baudRate_ ) * 3.5) * 1000 + 1;
    }

//    qDebug() << QString("Silence time: %1 ms").arg(silenceTime_);
//    emit infoMessage(tr("Silence time: %1 ms").arg(silenceTime_));
}

//-----------------------------------------------------------------------------

void
RTUDevice::setStopBits(QSerialPort::StopBits stopBits)
{
    if (stopBits_ != stopBits)
    {
        stopBits_ = stopBits;

        if (serialPort_->isOpen())
        {
            serialPort_->setStopBits(stopBits_);
        }
    }
}

//-----------------------------------------------------------------------------

void
RTUDevice::wait(int time)
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
