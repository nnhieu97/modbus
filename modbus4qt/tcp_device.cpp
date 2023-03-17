#include <QTcpSocket>

#include "memory_utils.h"
#include "tcp_device.h"

namespace modbus4qt
{

//-----------------------------------------------------------------------------

TCPDevice::TCPDevice(int port) :
    Device(),
    port_(port),
    transactionID_(0)
{
    tcpSocket_ = new QTcpSocket;
}

//-----------------------------------------------------------------------------

bool
TCPDevice::extractPDU(const QByteArray& buf, ProtocolDataUnit& pdu, ErrorCodes& errorCode)
{
    errorCode = NO_ERROR;

    qDebug() << "ADU: " << buf.toHex();
    qDebug() << "ADU size: " << buf.size();

    QByteArray tempBuf(buf);

    // For any case we should check size of recieved data to protect memory
    //
    if (tempBuf.size() >  int(sizeof(TCPApplicationDataUnit)))
    {
        tempBuf.resize(sizeof(TCPApplicationDataUnit));
    }

    int tempBufSize = tempBuf.size();

    // Minimum ADU size can be 9 bytes: 2 byte for Transaction ID, 2 bytes for Protocol ID,
    // 2 byte for Length, 1 byte for address, 2 byte for minimum PDU
    //
    if (tempBufSize < 9)
    {
        errorCode = TOO_SHORT_ADU;

        return false;
    }

    TCPApplicationDataUnit adu;

    WordRec headerTID;
    headerTID.bytes[0] = tempBuf[0];
    headerTID.bytes[1] = tempBuf[1];

    adu.tcpDataHeader.transactionId = headerTID.word;

    WordRec headerPID;
    headerPID.bytes[0] = tempBuf[2];
    headerPID.bytes[1] = tempBuf[3];

    adu.tcpDataHeader.protocolId = headerPID.word;

    WordRec headerLength;
    headerLength.bytes[0] = tempBuf[4];
    headerLength.bytes[1] = tempBuf[5];

    adu.tcpDataHeader.recLength = headerLength.word;

    adu.tcpDataHeader.unitId = tempBuf[6];

    adu.pdu.functionCode = tempBuf[7];

    for (int i = 8; i < tempBufSize; ++i)
    {
        adu.pdu.data[i - 8] = tempBuf[i];
    }

#ifndef QT_NO_DEBUG
    // If we want to print PDU into log file we should remove first 7 byte
    // After that we have pdu in tempBuf
    //
    tempBuf.remove(0, 7);
    qDebug() << "PDU: " << tempBuf.toHex();
    qDebug() << "PDU size: " << tempBuf.size();
#endif

    pdu = adu.pdu;

    return true;
}

//-----------------------------------------------------------------------------

QByteArray
TCPDevice::prepareADU_(const ProtocolDataUnit& pdu, int pduSize)
{
    QByteArray result;

    result.append((char*)&transactionID_, 2);

    result.append('\x0');
    result.append('\x0');

    result[4] = ((pduSize + 1) >> 8) & 0xFF;
    result[5] = (pduSize + 1) & 0xFF;

    result.append(unitID_);

    result.insert(7, (char*)&pdu, pduSize);

    return result;
}

//-----------------------------------------------------------------------------

int
TCPDevice::port() const
{
    return port_;
}

//-----------------------------------------------------------------------------

void
TCPDevice::setPort(int newPort)
{
    if (port_ != newPort)
    {
        port_ = newPort;

        if (tcpSocket_->isOpen())
        {
            tcpSocket_->close();
        }
        port_ = newPort;

        //tcpSocket_->bind();
    }
}

} //namespace modbus4qt

//-----------------------------------------------------------------------------
// EOF
