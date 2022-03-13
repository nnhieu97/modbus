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

#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#include "types.h"

class QIODevice;

namespace modbus4qt
{

/**
 * @brief
 * @en Base class for MODBUS device
 * @ru Базовый класс подключения к сети MODBUS произвольного устройства
 *
 * @en
 * Descendant class must realize abstract methods for reading and writing data from equipment.
 * Also it is important to initialize ioDevice_ field with address of real object responsible
 * for data exchange.
 *
 * @ru
 * Класс используется для подключения в сеть MODBUS внешнего устройства, которое не имеет такого функционала.
 * Может быть использован для реализации сервера MODBUS, работающего с устройствами, не поддерживающими протокол.
 *
 * Класс-потомок должен релизовать абстрактные методы обмена данными с оборудованием, а также правильно инициализировать
 * указатель ioDevice_, присвоив его значение указателю на экземпляр реального объекта, отвечающего
 * за обмен данными.
 */
class Device : public QObject
{
    Q_OBJECT

    friend class Server;

    protected:

        /**
         * @brief
         * @ru Устройство ввода-вывода, через которое идет обмен данными
         */
        QIODevice* ioDevice_;

        /**
         * @brief
         * @en Maximum allowed timeout for reading
         * @ru Максимальное время ожидания чтения данных
         *
         * @en Default value 5000 ms (5 sec).
         * @ru Значение по умолчанию 5000 мс (5 с).
         */
        int readTimeout_;

        /**
         * @brief
         * @en Maximum allowed timeout for writing
         * @ru Максимальное время ожидания записи данных
         *
         * @en Default value 5000 ms (5 sec).
         * @ru Значение по умолчанию 5000 мс (5 с).
         */
        int writeTimeout_;

    public:

        /**
         * @brief
         * @en Default constructor
         * @ru Конструктор по умолчанию
         *
         * @param
         * @en parent - parent object
         * @ru parent - указатель на объект-родитель
         */
        explicit Device(int readTimeout = 500, int writeTimeout = 500, QObject *parent = 0);

        inline int readTimeout() const;
        inline void setReadTimeout(int readTimeout);

        inline int writeTimeout() const;
        inline void setWriteTimeout(int writeTimeout);

    protected :

        virtual bool readCoil(quint16 regNo, bool &value) = 0;

        virtual bool readCoils(quint16 regStart, quint16 regQty, QVector<bool> &values) = 0;

        virtual bool readDescreteInput(quint16 regNo, bool &value) = 0;

        virtual bool readDescreteInputs(quint16 regStart, quint16 regQty, QVector<bool> &values) = 0;

        virtual bool readInputRegister(quint16 regNo, quint16 &value) = 0;

        virtual bool readInputRegisters(quint16 regStart, quint16 regQty, QVector<quint16> &values) = 0;

        virtual bool readHoldingRegister(quint16 regNo, quint16 &value) = 0;

        virtual bool readHoldingRegisters(quint16 regStart, quint16 regQty, QVector<quint16> &values) = 0;

        virtual bool writeCoil(quint16 regNo, bool value) = 0;

        virtual bool writeHoldingRegister(quint16 regNo, bool value) = 0;

    signals:

        /**
         * @brief
         * @en Signal for debuggin info
         * @ru Сигнал для вывода отладочного сообщения
         *
         * @param
         * @en msg - Debug message
         * @ru msg - Строка с отладочным сообщение
         */
        void debugMessage(const QString& msg);

        /**
         * @brief
         * @en Signal for informing about error occured
         * @ru Сигнал для сообщения о возникновении ошибки
         *
         * @param
         * @en msg - Message with error description
         * @ru msg - Строка с описанием ошибки
         */
        void errorMessage(const QString& msg);

        /**
         * @brief
         * @en Signal for informing about error occured
         * @ru Сигнал для сообщения о возникновении ошибки
         *
         * @param
         * @en msg - Message with error description
         * @ru msg - Строка с описанием ошибки
         */
        void infoMessage(const QString& msg);

    public slots:
};

//-----------------------------------------------------------------------------

int
Device::readTimeout() const
{
    return readTimeout_;
}

//-----------------------------------------------------------------------------

void
Device::setReadTimeout(int readTimeout)
{
    readTimeout_ = readTimeout;
}

//-----------------------------------------------------------------------------

void
Device::setWriteTimeout(int writeTimeout)
{
    writeTimeout_ = writeTimeout;
}

//-----------------------------------------------------------------------------

int
Device::writeTimeout() const
{
    return writeTimeout_;
}

//-----------------------------------------------------------------------------

}

#endif // DEVICE_H
