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


//namespace modbus4qt
//{

////!
////! \brief Default read/write timeout, 3000 ms
////!
//const int DEFAULT_TIMEOUT = 3000;

///**
// * @brief
// * @en Max size of protocol data unit, bytes
// * @ru Максимальный размер блока данных протокола, байт
// */
//const int PDUMaxSize = 253;

///**
// * @brief
// * @en Max size of data in protocol data unit, bytes.
// * @ru Максимальный размер массива данных в блоке данных протокола, байт
// */
//const int PDUDataMaxSize = PDUMaxSize - 1;

///**
// * @brief
// * @en Default port for MODBUS/TCP
// * @ru TCP порт для сервера MODBUS/TCP по умолчанию
// */
//const int DefaultTcpPort = 502;

///**
// * @brief
// * @en Unit ID which will be ignored (for MODBUS/TCP only)
// * @ru Значение номера устройства в сети, при котором номер устройства не используется
// */
//const quint8 IgnoreUnitId = 255;

///**
// * @brief
// * @en Unit ID for broadcast message (for MODBUS/TCP only)
// * @ru Номер устройства для широковещательной рассылки
// */
//const quint8 BroadcastUnitId = 0;

///**
// * @brief
// * @en Maximum coils quantity for reading
// * @ru Максимальное количество дискретных входов-выходов для пакетного чтения
// *
// * @en See also: Modbus Protocol Specification v1.1b3, p. 11
// * @ru Подробнее: Modbus Protocol Specification v1.1b3, стр. 11
// */
//const int MaxCoilsForRead = 2000;

///**
// * @brief
// * @en Maximum coils quantity for writing
// * @ru Максимальное количество дискретных выходов для пакетной записи
// *
// * @en See also: Modbus Protocol Specification v1.1b3, p. 29
// * @ru Подробнее: Modbus Protocol Specification v1.1b3, стр. 29
// */
//const int MaxCoilsForWrite = 1968;

///**
// * @brief
// * @en Maximum registers quantity for reading
// * @ru Максимальное количество читаемых значений регистров ввода/вывода для пакетного чтения
// *
// * @en  See also: Modbus Protocol Specification v1.1b3, p. 15
// * @ru Подробнее: Modbus Protocol Specification v1.1b3, стр. 15
// */
//const int MaxRegistersForRead = 125;

///**
// * @brief
// * @en Maximum registers quantity for writing
// * @ru Максимальное количество записываемых значений регистров вывода для пакетной записи
// *
// * @en  See also: Modbus Protocol Specification v1.1b3, p. 30
// * @ru Подробнее: Modbus Protocol Specification v1.1b3, стр. 30
// */
//const int MaxRegistersForWrite = 23;

////!
////*/! \brief Default timeout connection, ms
////!
//const int CONNECTION_TIMEOUT = 3000;
//;

//} // namespace modbus*/

#endif // CONSTS_H
