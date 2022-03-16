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

#ifndef MODBUS4QT_ABSTRACT_DEVICE_H
#define MODBUS4QT_ABSTRACT_DEVICE_H

#include <QObject>

#include "global.h"
#include "consts.h"
#include "types.h"

namespace modbus4qt
{

class AbstractDevice : public QObject
{
        Q_OBJECT
    public:

        enum ErrorCodes
        {
            NO_ERROR = 0,
            TOO_SHORT_ADU = 1,
            CRC_MISMATCH = 2

        };
        Q_ENUM(ErrorCodes)


        explicit AbstractDevice(QObject* parent = nullptr);

        //!
        //! \brief Process Modbus-RTU application data unit and return protocol data unit
        //! \param buf - array with data readed, should contain ADU record
        //! \return Protocol data unit
        //!
        static bool getPDUForRTU(const QByteArray& buf, ProtocolDataUnit& pdu, ErrorCodes& errorCode);
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
        //! \brief Signal for informing
        //! \param msg - message
        //!
        void infoMessage(const QString& msg);


    protected:

        /**
         * @brief
         * @en Forms protocol data unit
         * @ru Формирует блок данных (Application Data Unit)
         *
         * @en
         * Function should be realized in descendent class since format of application
         * data unit depends from physical commincation line used.
         *
         * @ru
         * Функция должна быть реализована в классе-потомке, так как формат блока
         * данных приложения зависит от используемого канала передачи данных.
         *
         * @param
         * @en pdu - Protocol Data Unit
         * @ru pdu - блок данных протокола (Protocol Data Unit)
         *
         * @param
         * @en pduSize - size of protocol data unit (in bytes)
         * @ru pduSize - размер в байтах блока данных протокола
         *
         * @return
         * @en Application Data Unit formed
         * @ru Сформированный блок данных приложения (Application Data Unit)
         *
         * @sa ProtocolDataUnit, RTUApplicationDataUnit
         *
         */
        virtual QByteArray prepareADU_(const ProtocolDataUnit& pdu, int pduSize) = 0;

        /**
         * @brief
         * @en Process application data unit recieved from server and returns protocol data unit
         * @ru Разбирает полученный от сервера блок данных (Application Data Unit) и возвращает блок данных протокола MODBUS (Protocol Data Unit).
         *
         * @en
         * Function should be realized in descendent class since format of application
         * data unit depends from physical commincation line used.
         *
         * @ru
         * Функция должна быть реализована в классе-потомке, так как формат блока
         * данных приложения зависит от используемого канала передачи данных.
         *
         * @param
         * @en buf - data recieved from server
         * @ru buf - полученный массив данных, полученный от устройства севера
         *
         * @return
         * @en Protocol data unit
         * @ru Блок данных протокола MODBUS
         *
         * @sa ProtocolDataUnit, RTUApplicationDataUnit
         *
         */
        virtual ProtocolDataUnit processADU_(const QByteArray& buf) = 0;

        /**
         * @brief
         * @en Read response from slave device
         * @ru Читает ответ от сервера MODBUS
         *
         * @return
         * @en Readed data array
         * @ru Массив прочитанных данных
         *
         * @en As timeout processing is differ for RTU and TCP we need to implement
         * method in descendance.
         * @ru Так как обработка времени задержки чтения разная для RTU и TCP
         * метод должен быть реализован в классе-потомке.
         */
        virtual QByteArray readResponse_() = 0;

};

} // namespace modbus4qt

#endif // MODBUS4QT_ABSTRACT_DEVICE_H
