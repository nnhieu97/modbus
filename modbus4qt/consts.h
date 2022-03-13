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

#ifndef CONSTS_H
#define CONSTS_H

#include <QtGlobal>

/**
 * Constants which defines the format of a modbus frame. The example is
 * shown for a Modbus RTU/ASCII frame. Note that the Modbus PDU is not
 * dependent on the underlying transport.
 *
 * <code>
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
 * </code>
 */


/*
 *
 * <------------------------ MODBUS TCP/IP ADU(1) ------------------------->
 *              <----------- MODBUS PDU (1') ---------------->
 *  +-----------+---------------+------------------------------------------+
 *  | TID | PID | Length | UID  |Code | Data                               |
 *  +-----------+---------------+------------------------------------------+
 *  |     |     |        |      |
 * (2)   (3)   (4)      (5)    (6)
 *
 * (2)  ... MB_TCP_TID          = 0 (Transaction Identifier - 2 Byte)
 * (3)  ... MB_TCP_PID          = 2 (Protocol Identifier - 2 Byte)
 * (4)  ... MB_TCP_LEN          = 4 (Number of bytes - 2 Byte)
 * (5)  ... MB_TCP_UID          = 6 (Unit Identifier - 1 Byte)
 * (6)  ... MB_TCP_FUNC         = 7 (Modbus Function Code)
 *
 * (1)  ... Modbus TCP/IP Application Data Unit
 * (1') ... Modbus Protocol Data Unit
 */


namespace modbus4qt
{

//!
//! \brief Default read/write timeout, 3000 ms
//!
const int DEFAULT_TIMEOUT = 3000;

/**
 * @brief
 * @en Max size of protocol data unit, bytes
 * @ru Максимальный размер блока данных протокола, байт
 */
const int PDUMaxSize = 253;

/**
 * @brief
 * @en Max size of data in protocol data unit, bytes.
 * @ru Максимальный размер массива данных в блоке данных протокола, байт
 */
const int PDUDataMaxSize = PDUMaxSize - 1;

/**
 * @brief
 * @en Default port for MODBUS/TCP
 * @ru TCP порт для сервера MODBUS/TCP по умолчанию
 */
const int DefaultTcpPort = 502;

/**
 * @brief
 * @en Unit ID which will be ignored (for MODBUS/TCP only)
 * @ru Значение номера устройства в сети, при котором номер устройства не используется
 */
const quint8 IgnoreUnitId = 255;

/**
 * @brief
 * @en Unit ID for broadcast message (for MODBUS/TCP only)
 * @ru Номер устройства для широковещательной рассылки
 */
const quint8 BroadcastUnitId = 0;

/**
 * @brief
 * @en Maximum coils quantity for reading
 * @ru Максимальное количество дискретных входов-выходов для пакетного чтения
 *
 * @en See also: Modbus Protocol Specification v1.1b3, p. 11
 * @ru Подробнее: Modbus Protocol Specification v1.1b3, стр. 11
 */
const int MaxCoilsForRead = 2000;

/**
 * @brief
 * @en Maximum coils quantity for writing
 * @ru Максимальное количество дискретных выходов для пакетной записи
 *
 * @en See also: Modbus Protocol Specification v1.1b3, p. 29
 * @ru Подробнее: Modbus Protocol Specification v1.1b3, стр. 29
 */
const int MaxCoilsForWrite = 1968;

/**
 * @brief
 * @en Maximum registers quantity for reading
 * @ru Максимальное количество читаемых значений регистров ввода/вывода для пакетного чтения
 *
 * @en  See also: Modbus Protocol Specification v1.1b3, p. 15
 * @ru Подробнее: Modbus Protocol Specification v1.1b3, стр. 15
 */
const int MaxRegistersForRead = 125;

/**
 * @brief
 * @en Maximum registers quantity for writing
 * @ru Максимальное количество записываемых значений регистров вывода для пакетной записи
 *
 * @en  See also: Modbus Protocol Specification v1.1b3, p. 30
 * @ru Подробнее: Modbus Protocol Specification v1.1b3, стр. 30
 */
const int MaxRegistersForWrite = 23;

//!
//! \brief Default timeout connection, ms
//!
const int CONNECTION_TIMEOUT = 3000;

/**
 * @brief
 * @en MODBUS error codes
 * @ru Коды функций протокола MODBUS
 */
struct Exceptions
{
    /**
     * @brief
     * @en No error occured
     * @ru Нет ошибок
     */
    static const quint8 Ok = 0x00;

    /**
     * @brief
     * @en Illegal function
     * @ru Неверная функция
     */
    static const quint8 IllegalFunction = 0x01;

    /**
     * @brief
     * @en Illegal data address
     * @ru Неверный номер регистра
     */
    static const quint8 IllegalDataAddress = 0x02;

    /**
     * @brief
     * @en Illegal data value
     * @ru Неверное значение
     */
    static const quint8 IllegalDataValue = 0x03;

    /**
     * @brief
     * @en Server device falilure
     * @ru Общий сбой сервера
     */
    static const quint8 ServerDeviceFailure = 0x04;

    /**
     * @brief
     * @en Acknowledge
     * @ru Acknowledge - не знаю, что это значит :-)
     */
    static const quint8 Acknowledge = 0x05;

    /**
     * @brief
     * @en Server device busy
     * @ru Сервер занят
     */
    static const quint8 ServerDeviceBusy = 0x06;

    /**
     * @brief
     * @en Memory parity error
     * @ru Ошибка памяти
     */
    static const quint8 MemoryParityError = 0x08;

    /**
     * @brief
     * @en Gateway path not available
     * @ru Не найден путь к шлюзу
     */
    static const quint8 GatewayPathNotAvailable = 0x0A;

    /**
     * @brief
     * @en Gateway target device failed to response
     * @ru Нет ответа от усройства через шлюз
     */
    static const quint8 GatewayTargetDeviceFailedToResponse = 0x0B;
};

/**
 * @brief
 * @en MODBUS function codes
 * @ru Коды функций MODBUS
 */
struct Functions
{
    /**
     * @brief
     * @en Read data block from coils table
     * @ru Читать блок данных из таблицы дискретных выходов (Coils)
     */
    static const quint8 ReadCoils = 0x01;

    /**
     * @brief
     * @en Read data block from discrete inputs table
     * @ru Читать блок данных из таблицы дискретных входов (Descrete Input)
     */
    static const quint8 ReadDescereteInputs = 0x02;

    /**
     * @brief
     * @en Read data block from holding registers table
     * @ru Читать блок данных из таблицы регистров вывода (Holding Registers)
     */
    static const quint8 ReadHoldingRegisters = 0x03;

    /**
     * @brief
     * @en Read data block from input registers table
     * @ru Читать блок данных из таблицы регистров ввода (Input Registers)
     */
    static const quint8 ReadInputRegisters = 0x04;

    /**
     * @brief
     * @en Write single value to coils table
     * @ru Записать значение в таблицу дискретных выходов (Coils)
     */
    static const quint8 WriteSingleCoil = 0x05;

    /**
     * @brief
     * @en Write single value to holding registers table
     * @ru Записать значение в таблицу регистров вывывода (Holding Registers)
     */
    static const quint8 WriteSingleRegister = 0x06;

//      Commented function codes not implemeted for now!
//
//    /**
//     * @brief Чтение содержимого of Eight Exception Status outputs на удаленном устройстве.
//     *
//     * Используется только при передаче данных через последовательную линию связи.
//     *
//     */
//    static const quint8 ReadExceptionStatus = 0x07;

//    /**
//     * @brief Провести серию тестов для проверки системы передачи данных между клиентом и сервером.
//     *
//     * Используется только при передаче данных через последовательную линию связи.
//     *
//     */
//    static const quint8 Diagnostics = 0x08;

//    /**
//     * @brief Получить значение состояния и счетчик событий удаленного устройства.
//     *
//     * Используется только при передаче данных через последовательную линию связи.
//     *
//     */
//    static const quint8 GetCommEventCounter = 0x0B;

//    /**
//     * @brief Получить значение состояния, счетчик событий, счетчик сообщений и поле с байтами событий
//     *
//     * Используется только при передаче данных через последовательную линию связи.
//     *
//     */
//    static const quint8 GetCommEventLog = 0x0C;

    /**
     * @brief
     * @en Write data block to coils table
     * @ru Записать блок данных в таблицу дискретных выходов (Coils)
     */
    static const quint8 WriteMultipleCoils = 0x0F;

    /**
     * @brief
     * @en Write data block from holding registers table
     * @ru Записать блок данных в таблицу регистров вывода (Holding Registers)
     */
    static const quint8 WriteMultipleRegisters = 0x10;

//    /**
//     * @brief Прочитать описание типа, текущее состояние и другую информацию, специфичную для удаленного устройства.
//     *
//     * Используется только при передаче данных через последовательную линию связи.
//     *
//     */
//    static const quint8 ReportServerID = 0x11;

//    /**
//     * @brief ReadFileRecord
//     */
//    static const quint8 ReadFileRecord = 0x14;

//    /**
//     * @brief WriteFileRecord
//     */
//    static const quint8 WriteFileRecord = 0x15;

//    /**
//     * @brief MaskWriteReg
//     */
//    static const quint8 MaskWriteRegister = 0x16;

//    /**
//     * @brief ReadWriteRegs
//     */
//    static const quint8 ReadWritMultipleeRegisters = 0x17;

//    /**
//     * @brief ReadFiFoQueue
//     */
//    static const quint8 ReadFIFOQueue = 0x18;

//    /**
//     * @brief EncapsulatedInterfaceTransport
//     */
//    static const quint8 EncapsulatedInterfaceTransport = 0x2B;
};

} // namespace modbus

#endif // CONSTS_H
