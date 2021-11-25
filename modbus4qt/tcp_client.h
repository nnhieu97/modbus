/*****************************************************************************
 * modbus4qt Library
 * Copyright (C) 2012 MELZ-Invest JSC
 * http://www.melz-invest.ru/projects/modbus4qt
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 *****************************************************************************/

/*

The contents of this file are subject to the Mozilla Public License Version 1.1
(the "License"); you may not use this file except in compliance with the
License. You may obtain a copy of the License at http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
the specific language governing rights and limitations under the License.

Alternatively, the contents of this file may be used under the terms of the
GNU General Public License Version 2 or later (the "GPL"), in which case
the provisions of the GPL are applicable instead of those above. If you wish to
allow use of your version of this file only under the terms of the GPL and not
to allow others to use your version of this file under the MPL, indicate your
decision by deleting the provisions above and replace them with the notice and
other provisions required by the GPL. If you do not delete the provisions
above, a recipient may use your version of this file under either the MPL or
the GPL.

This file based on: IdModbusClient.pas,v 1.31 2011/07/04 13:05:27 plpolak Exp $
from delhimodbus-1.5.0

*/

#ifndef MODBUS_TCP_CLIENT_H
#define MODBUS_TCP_CLIENT_H


#include "client.h"

#include <QHostAddress>
#include <QTcpSocket>
#include <QVector>

namespace modbus4qt
{

//! Класс MODBUS/Tcp клиента
class MODBUS4QT_EXPORT TcpClient : public Client
{
    Q_OBJECT

    private: // Закрытые поля класса

        //! Использовать или нет режим автоматического подключения к серверу
        /**
            @ru Значение по умолчанию: истина
        */
        bool autoConnect_;

        //! Максимальное время ожидания подключения к серверу в милисекундах
        /**
          Значение по умолчанию CONNECTION_TIMEOUT 3000 ms (3 сек).
        */
        int connectTimeOut_;


        //! Номер TCP порта для связи с сервером
        /**
            Значение по умолчанию: 502
            Исходное объявление: property Port default MB_PORT;
        */
        int port_;

        //! IP адрес сервера
        /**
            Значение по умполчанию 127.0.0.1
        */
        QHostAddress serverAddress_;

        //! Внутренняя переменная для хранения указателя на ioDevice.
        /**
          Используется для обеспечения возможности вызова методов TcpSocket,
          экземпляром которого является ioDevice в контексте данного класса.
        */
        QTcpSocket* tcpSocket_;

        //! Номер последней транзакции
        /**
            Исходное объявление: FLastTransactionID: Word;
        */
        quint16 lastTransactionID_;

    private:
        //! Возвращает номер следующей транзакции
        /**
            Увеличивает номер последней транзакции на единицу и возщвращает полученное значение.
            Если номер транзакции рhttp://ru.wikipedia.org/wiki/%C1%EE%E4авен 0xFFFF, то возвращает 0.

            Готовность: полностью готова <br>
            Исходное объявление: function GetNewTransactionID: Word;
        */
        quint16 getNewTransactionID_();

        virtual bool sendRequestToServer_(const ProtocolDataUnit& requestPDU,  int requestPDUSize, ProtocolDataUnit* responsePDU) override;

    public:

        //! Конструктор по умолчанию
        /**
            Инициализирует внутренние переменные экземпляра класса.

            Готовность: почти готов <br>
            Исходное объявление: {$IFNDEF DMB_INDY10} constructor Create(AOwner: TComponent); override; {$ENDIF}

            @todo Присвоить значения указателям обработчиков ошибок
        */
        explicit TcpClient(QObject *parent = 0);

        //! Выполняет подключение к серверу
        virtual bool connectToServer(int timeout = CONNECTION_TIMEOUT);

        //! Отключается от сервера
        virtual void disconnectFromServer();

        //! Возвращает значение режима автоматического подключения к серверу
        bool isAutoConnect() const;

        //! Проверяет наличие подключения к серверу
        /**
            Возвращает true, если клиент подключен к серверу. false - в противном случае.
        */
        virtual bool isConnected() const;

        //! Возвращает текущее значение адреса сервера
        QHostAddress getServerAddress() const;

        //! Устанавливает режим автоматического подключения к серверу
        void setAutoConnect(bool autoConnect = true);

        //! Устанавливает адрес сервера для подключения
        /**
            Если ранее было установлено соединение с другим сервером, то соединение будет закрыто.
        */
        void setServerAddress(const QHostAddress& getServerAddress);

        // Client interface
    protected:

        virtual QByteArray prepareADU_(const ProtocolDataUnit& pdu, int pduSize) override;

        virtual ProtocolDataUnit processADU_(const QByteArray& buf) override;

        virtual QByteArray readResponse_() override;
};

} // namespace modbus

#endif // MODBUS_TCP_CLIENT_H
