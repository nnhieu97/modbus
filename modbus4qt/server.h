#ifndef SERVER_H
#define SERVER_H

/*****************************************************************************
 * modbus4qt Library
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 * Copyright (C) 2012-2015
 * http://www.modbus4qt.ru
 *
 * $Id: server.h 174 2016-04-27 17:43:13Z l.kolesnik $
 * $URL: https://o.m-i.ru/svn/modbus4qt/branches/1.1/src/server.h $
 *****************************************************************************/

#include <QObject>

#include "global.h"
#include "consts.h"
#include "types.h"

class QIODevice;

namespace modbus4qt
{

class Device;

/**
* @brief
* @en Abtract modbus server
* @ru Абстрактный сервер протокола modbus
 */
class Server : public QObject
{
    Q_OBJECT

    private:
        /**
         * @brief
         * @en device_
         * @ru Исполнительное устройство
         */
        Device *device_;

        /**
         * @brief
         * @en IO device for data exchange with client
         * @ru Устройство ввода-вывода, через которое идет обмен данными с клиентом
         */
        QIODevice* ioDevice_;

        /**
         * @brief
         * @en Maximum allowed timeout for reading data
         * @ru Максимальное время ожидания чтения данных
         *
         * @en Default value 5000 ms (5 sec).
         * @ru Значение по умолчанию 5000 мс (5 с).
         */
        int readTimeout_;

        /**
         * @brief
         * @en Maximum allowed timeout for sending data
         * @ru Максимальное время ожидания записи данных
         *
         * @en Default value 5000 ms (5 sec).
         * @ru Значение по умолчанию 5000 мс (5 с).
         */
        int writeTimeout_;

        /**
         * @brief
         * @en Identifier number of server
         * @ru Идентификатор сервера
         */
        quint8 unitID_;

    public:

        /**
         * @brief
         * @en Default constructor
         * @ru Конструктор по умолчанию
         *
         * @param
         * @en parent - parent object
         * @ru parent - указатель на объект-родитель.
         */
        explicit Server(QObject *parent = 0);

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
};

} // namespace modbus4qt



#endif // SERVER_H
