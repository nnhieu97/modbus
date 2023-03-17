#ifndef RTUDEVICE_H
#define RTUDEVICE_H

#include <QSerialPort>

#include "device.h"

namespace modbus4qt
{

class RTUDevice : public Device
{
    public:

        #pragma pack(push, 1)

        //!
        //! \brief Application data unit for MODBUS over serial line
        //!
        struct RTUApplicationDataUnit
        {
            //!
            //! \brief Address of device
            //!
            //! Should be from 1 to 247. Range 248-255 is reserved by protocol specification.
            //!
            uint8_t unitId;

            //!
            //! \brief Protocol data unit
            //!
            //! \sa ProtocolDataUnit
            //!
            ProtocolDataUnit pdu;

            //!
            //! \brief Error checking field is the result of a "Redundancy Checking"
            //!
            //! CRC calculation is performed on the message content.
            //!
            uint16_t crc;
        };

        #pragma pack(pop)

        //!
        //! \brief RTUDevice - default constructor for RTUDevice class.
        //! \param portName - port name
        //! \param baudRate - baud rate
        //! \param dataBits - data bits
        //! \param stopBits - stop bits
        //! \param parity - parity
        //!
        explicit RTUDevice(const QString& portName,
                           const QSerialPort::BaudRate& baudRate = QSerialPort::Baud9600,
                           const QSerialPort::DataBits& dataBits = QSerialPort::Data8,
                           const QSerialPort::StopBits& stopBits = QSerialPort::OneStop,
                           const QSerialPort::Parity& parity = QSerialPort::EvenParity);

        //!
        //! \brief ~RTUDevice - destructor.
        //!
        //! Close and delete serial port object.
        //!
        virtual ~RTUDevice();

        //!
        //! \brief If port is open set data exchange settings with current values.
        //! \return true in success and flase overwise
        //!
        //! If port is not open errorMessage() signal will be emited.
        //!
        virtual bool configurePort_();

        //!
        //! \brief Calculate the checksum of the data buffer for the crc16 algorithm
        //! \param buf - buffer to calculate crc
        //! \return CRC calculated
        //!
        //! The code of this function was taken from <a href="http://www.libmodbus.org">libmodbus</a>
        //!
        uint16_t crc16(const QByteArray& buf);

        //!
        //! \brief Extract protocol data unit from application data unit.
        //! \param buf - array with data readed, should contain ADU record
        //! \param pdu - pdu extracted
        //! \param errorCode - error code if some error occured
        //! \return true in success and flase overwise
        //!
        virtual bool extractPDU(const QByteArray& buf, ProtocolDataUnit& pdu, ErrorCodes& errorCode) override;

        //!
        //! \brief Open port and set up it with current data exchange settings.
        //! \return true, if port opened succesfullty; false in case of error
        //!
        //! In case of error error message will be setted.
        //!
        bool openPort();

        //!
        //! \brief Return current port name
        //! \return Current port name
        //!
        QString portName() const;

        //!
        //! \brief Prepare application data unit for MODBUS/RTU specification.
        //! \param pdu - protocol data unit
        //! \param pduSize - size of protocol data unit
        //! \return Arruy containing application data unit
        //!
        virtual QByteArray prepareADU_(const ProtocolDataUnit& pdu, int pduSize);

        //!
        //! \brief serialPort - return pointer to serial port object.
        //! \return Pointer to serial port object
        //!
        QSerialPort* serialPort() const;

        //!
        //! \brief Set up baud rate.
        //! \param baudRate - new baud rate
        //!
        //! If port is open then baud rate will be changed on fly.
        //!
        void setBaudRate(QSerialPort::BaudRate baudRate);

        //!
        //! \brief Set up quantity of data bits.
        //! \param dataBits - quantity of data bits
        //!
        //! If port is open then data bits quantity will be changed on fly.
        //!
        void setDataBits(QSerialPort::DataBits dataBits);

        //!
        //! \brief Set up type of parity control.
        //! \param parity - type of parity control
        //!
        //! If port is open then parity control will be changed on fly.
        //!
        void setParity(QSerialPort::Parity parity);

        //!
        //! \brief Set up port name for data exchange
        //! \param portName - new port name
        //!
        //! If port is open it will be closed. You should open it by hand.
        //!
        void setPortName(const QString& portName);

        //!
        //! \brief Set up quantity of stop bits.
        //! \param stopBits - quantity of stop bits
        //!
        //! If port is open then quantity of stop bits will be changed on fly.
        //!
        void setStopBits(QSerialPort::StopBits stopBits);

        //!
        //! \brief Set current value of unit ID
        //! \param unitID - new unit ID
        //!
        void setUnitID(quint8 unitID);

        //!
        //! \brief Return current value of unit ID
        //! \return unit id
        //!
        uint8_t unitID() const;

        /**
          * @brief
          * @en Wait time milliseconds
          * @ru Сделать паузу в выполнении
          *
          * @param
          * @en time - pause time, milliseconds
          * @ru time - время задержки в миллисекундах
          */
        void wait(int time);


    protected: // fields

        //!
        //! \brief Name of port using for data exchange
        //!
        QString portName_;

        //!
        //! \brief Port for data exchange
        //!
        QSerialPort* serialPort_;

        //!
        //! \brief Baud rate
        //!
        //! Default value QSerialPort::Baud9600
        //!
        QSerialPort::BaudRate baudRate_;

        //!
        //! \brief Quantity of data bits
        //!
        //! Default value: QSerialPort::Data8
        //!
        QSerialPort::DataBits dataBits_;

        //!
        //! \brief Quantity of stop bits
        //!
        //! Default value: QSerialPort::OneStop
        //!
        QSerialPort::StopBits stopBits_;

        //!
        //! \brief Parity mode
        //!
        //! Default value: QSerialPort::EvenParity
        //!
        QSerialPort::Parity parity_;

        //!
        //! \brief Time of silence interval, ms
        //!
        int silenceTime_;

    protected: // methods

        //!
        //! \brief Set silence time depending of current baud rate.
        //!
        void setSilenceTime_();
};

} // namespace modbus4qt

#endif // RTUDEVICE_H
