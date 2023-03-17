#ifndef TCPDEVICE_H
#define TCPDEVICE_H

#include "device.h"

class QTcpSocket;

namespace modbus4qt
{

class TCPDevice : public Device
{
    public:

        #pragma pack(push, 1)

        //!
        //! \brief The TCPApplicationDataUnit struct
        //!
        struct TCPApplicationDataUnit
        {

            struct TCPDataHeader
            {
                //!
                //! \brief Transaction ID
                //!
                uint16_t transactionId;

                //!
                //! \brief Protocol ID
                //!
                uint16_t protocolId;

                //!
                //! \brief Data packet length
                //!
                uint16_t recLength;

                //!
                //! \brief Address of device
                //!
                //! Should be from 1 to 247. Range 248-255 is reserved by protocol specification.
                //!
                uint8_t unitId;

            } tcpDataHeader;

            //!
            //! \brief Protocol data unit
            //!
            //! \sa ProtocolDataUnit
            //!
            ProtocolDataUnit pdu;
        };

        #pragma pack(pop)

        explicit TCPDevice(int port = DEFAULT_TCP_PORT);

        //!
        //! \brief Extract protocol data unit from application data unit.
        //! \param buf - array with data readed, should contain ADU record
        //! \param pdu - pdu extracted
        //! \param errorCode - error code if some error occured
        //! \return true in success and flase overwise
        //!
        virtual bool extractPDU(const QByteArray& buf, ProtocolDataUnit& pdu, ErrorCodes& errorCode) override;

        //!
        //! \brief Prepare application data unit for MODBUS/RTU specification.
        //! \param pdu - protocol data unit
        //! \param pduSize - size of protocol data unit
        //! \return Arruy containing application data unit
        //!
        QByteArray prepareADU_(const ProtocolDataUnit& pdu, int pduSize);

        //!
        //! \brief Return TCP port number
        //! \return TCP port number
        //!
        int port() const;

        //!
        //! \brief Set new value to TCP port number
        //! \param newPort - new port number value
        //!
        void setPort(int newPort);

    protected : // fields

        //!
        //! \brief TCP socket for data exchange
        //!
        QTcpSocket* tcpSocket_;

        //!
        //! \brief Number of TCP port
        //!
        int port_;

        //!
        //! \brief Current transaction ID
        //!
        uint16_t transactionID_;

};

}

#endif // TCPDEVICE_H
