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

#ifndef TYPES_H
#define TYPES_H

#include <QtGlobal>
#include <QDebug>

#include <algorithm>

#include "consts.h"

namespace modbus4qt
{

/**
    @en Union for addressing bytes of word.
    @ru Стурктура для обращения к отдельным байтам двухбайтного слова.
*/
union WordRec
{
    quint16 word;
    quint8 bytes[2];
};

/**
* @brief
* @en Protocol Data Unit.
* @ru Блок данных протокола MODBUS.
*
* @en Protocol data unit is used in all types of communication lines.
* @ru Используется вне зависимости от способа передачи данных.
*
* @en See: Modbus Protocol Specification v1.1b3, page 5
* @ru Подробнее: Modbus Protocol Specification v1.1b3, стр. 5
*/
#pragma pack(1)
struct ProtocolDataUnit
{
    /**
     * @brief
     * @en modbus function code
     * @ru Код функции modbus.
     */
    quint8 functionCode;

    /**
     * @brief
     * @en Data to send
     * @ru Передаваемые данные
     */
    quint8 data[PDUDataMaxSize];

    /**
     * @brief
     * @en Default constructor. Fills data array with zeros.
     * @ru Конструктор по умолчанию. Заполняет массив данных нулями.
     */
    ProtocolDataUnit()
        : functionCode(0)
    {
        std::fill(data, data + PDUDataMaxSize, 0);
    }

    /**
     * @brief
     * @en Copying constructor. Copies data array from rhv.
     * @ru Конструктор копирования. Копирует данные из rhv.
     *
     * @param rhv
     */
    ProtocolDataUnit(const ProtocolDataUnit& rhv)
        : functionCode(rhv.functionCode)
    {
        std::copy(rhv.data, rhv.data + PDUDataMaxSize, data);
    }

    /**
     * @brief
     * @en operator =. Copies data array from rhv.
     * @ru Оператор копирования. Копирует данные из rhv.
     *
     * @param rhv
     *
     * @return
     * @en Reference to copy result
     * @ru Ссылка на результат копирования
     */
    ProtocolDataUnit& operator=(const ProtocolDataUnit& rhv)
    {
        functionCode = rhv.functionCode;
        std::copy(rhv.data, rhv.data + PDUDataMaxSize, data);

        return *this;
    }
};
#pragma pack()

/**
 * @brief
 * @en Application data unit for MODBUS over serial line.
 * @ru Блок данных протокола для протокола, использующего для передачи последовательную линии связи.
 */
#pragma pack(1)
struct RTUApplicationDataUnit
{
    /**
     * @brief
     * @en Address of slave device. Should be from 1 to 247. Range 248-255 is reserved.
     * @ru Иденитификатор устройства получателя. Должен быть лежать в диапазоне от 1 до 247. Значения 248-255 зарезервированы спецификацией протокола.
     */
    quint8 unitId;

    /**
     * @brief
     * @en Protocol data unit.
     * @ru Блок данных протокола.
     *
     * @sa ProtocolDataUnit
     */
    ProtocolDataUnit pdu;

    /**
     * @brief
     * @en Error checking field is the result of a "Redundancy Checking" calculation that is performed on the message contents.
     * @ru Поле контроля целостности пакета. Расчитывается по алгоритму crc16.
     */
    quint16 crc;
};
#pragma pack()

// TCP subsystem is not ready and should be rechecked!

/**
 * @brief The TcpDataHeader struct
 */
#pragma pack(1)
struct TcpDataHeader
{
    /**
     * @brief
     * @en Transaction ID
     * @ru Номер транзакции
     */
    quint16 transactionId;

    /**
     * @brief
     * @en Protocol ID
     * @ru Идентификтор протокола
     */
    quint16 protocolId;

    /**
     * @brief
     * @en Data packet length
     * @ru Длина пакета данных
     */
    quint16 recLength;

    //! Адрес подчинённого устройства, к которому адресован запрос
    /**
        Обычно игнорируется, если соединение установлено с конкретным устройством.
        Может использоваться, если соединение установлено с мостом,
        который выводит нас, например, в сеть RS485.
    */
    quint8 unitId;
};
#pragma pack()

/**
 * @brief The TCPApplicationDataUnit struct
 */
#pragma pack(1)
struct TCPApplicationDataUnit
{
    //! @todo Это пока не важно, надо исправить в соответствие со специйикацией
    quint8 unitId;
    ProtocolDataUnit pdu;
    quint16 crc;
};
#pragma pack()


#pragma pack(1)
//! Структура, описывающая буфер для запроса серверу
/**
Исходное объявление:
@code
type
  TModBusRequestBuffer = packed record
    Header: TModBusHeader;
    FunctionCode: TModBusFunction;
    MBPData: TModBusDataBuffer;
  end;
@endcode
*/
struct RequestBuffer
{
    //! Заголовок
    TcpDataHeader header;

    //! Код фукнции
    quint8 functionCode;

    //! Данные
    quint8 mbpData[261];
};
#pragma pack()

#pragma pack(1)
//! Структура, описывающая буфер ответа от сервера
/**
Исходное объявление:
@code
type
  TModBusResponseBuffer = packed record
    Header: TModBusHeader;
    FunctionCode: TModBusFunction;
    MBPData: TModBusDataBuffer;
  end;
@endcode
*/
struct ResponseBuffer
{
    //! Заголовок
    TcpDataHeader header;

    //! Код фукнции
    quint8 functionCode;

    //! Данные
    quint8 data[261];
};
#pragma pack()

/*
type
  TModBusExceptionBuffer = packed record
    Header: TModBusHeader;
    ExceptionFunction: TModBusFunction;
    ExceptionCode: Byte;
  end;
*/
#pragma pack(1)
struct ExceptionBuffer
{
    TcpDataHeader header;
    quint8 function;
    quint8 exceptionCode;
};
#pragma pack()

//! Массив для хранения и передачи данных регистров
/**
  Исходное объявление:
    @code
        type
          TModRegisterData = array[0..MaxBlockLength] of Word;
    @endcode
*/
//typedef quint16 RegisterData[MaxBlockLength];

//! Массив для хранения и передачи данных флагов
/**
  Исходное объявление:
    @code
        type
          TModCoilData = array[0..MaxCoils] of ByteBool;
    @endcode
*/
//typedef quint8 CoilData[MaxCoilsForRead];

//typedef QVector<bool> BitsArray;
//typedef QVector<quint16> WordsArray;


} // namespace modbus

#endif // TYPES_H
