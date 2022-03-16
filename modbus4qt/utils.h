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

#ifndef UTILS_H
#define UTILS_H

#include <QByteArray>
#include <QVector>
#include <QMutex>
#include <QWaitCondition>

#include "types.h"

namespace modbus4qt
{

//!
//! \brief Calculate the checksum of the data buffer for the crc16 algorithm
//! \param buf - buffer to calculate crc
//! \return CRC calculated
//!
//! The code of this function was taken from из <a href="http://www.libmodbus.org">libmodbus</a>
//!
quint16 crc16(const QByteArray& buf);

/**
 * @brief
 * @en Process coils data readed from server and returns values of coils as array
 * @ru Разбирает полученный от буфер с данными и записывает значения дискретных входов/выходов в массив
 *
 * @param
 * @en buffer - data recieved from server
 * @ru buffer - полученный от сервера буфер с данными
 *
 * @param
 * @en regQty - quintity of coils expected
 * @ru regQty - количество ожидаемых флагов
 *
 * @return
 * @en Array of coils
 * @ru Массив флагов
 */
QVector<bool> getCoilsFromBuffer(const QByteArray& buffer, quint16 regQty);

/**
 * @brief
 * @en Process register data readed from server and returns values as array
 * @ru Разбирает полученный от буфер с данными и записывает значения регистров в массив
 *
 * @param
 * @en buffer - data recieved from server
 * @ru buffer - полученный от сервера буфер с данными
 *
 * @param
 * @en regQty - quintity of coils expected
 * @ru regQty - количество ожидаемых флагов
 *
 * @return
 * @en Array of values
 * @ru Массив значений регистров
 */
QVector<quint16> getRegistersFromBuffer(const QByteArray& buffer, quint16 regQty);

/**
 * @brief
 * @en Puts coils data into protocol data buffer
 * @ru Записывает значения флагов в буфер данных протокола
 *
 * @param
 * @en buffer - data will be writed to protocol data unit
 * @ru buffer - буфер с данными для блока данных протокола (PDU)
 *
 * @param
 * @en data - coils values
 * @ru data - массив значений флагов
 */

void putCoilsIntoBuffer(quint8* buffer, const QVector<bool>& data);
/**
 * @brief
 * @en Puts registers data into protocol data buffer
 * @ru Записывает значения регистров в буфер данных протокола
 *
 * @param
 * @en buffer - data will be writed to protocol data unit
 * @ru buffer - буфер с данными для блока данных протокола (PDU)
 *
 * @param
 * @en data - registers values
 * @ru data - массив значений регистров
 */
void putRegistersIntoBuffer(quint8* buffer, const QVector<quint16>& data);

/**
  * @brief
  * @en Retrun hi byte of word
  * @ru Возвращает старший байт двухбайтного целого числа
  *
  * @param
  * @en word
  * @ru word - 2-х байтное слово
  *
  * @return
  * @en Hi byte of word
  * @ru Старший байт двухбайтного целого числа
  */
quint8 hi(quint16 word);

/**
  * @brief
  * @en Retrun low byte of word
  * @ru Возвращает младшиц байт двухбайтного целого числа
  *
  * @param
  * @en word
  * @ru word - 2-х байтное слово
  *
  * @return
  * @en Low byte of word
  * @ru Младший байт двухбайтного целого числа
  */
quint8 lo(quint16 word);

/**
  * @brief
  * @en Swap two bytes in word and return result
  * @ru Переставляет местами байты в 2-х байтном целом и возвращает результат
  *
  * @param
  * @en word
  * @ru word - 2-х байтное слово
  *
  * @return
  * @en Word with swapped bytes
  * @ru Слово с переставленными байтами
  */
quint16 swap(quint16 word);

/**
  * @brief
  * @en Convert 2 bytes word from local byte order to net byte order
  * @ru Конвертирует 16-битную беззнаковую величину из локального порядка байтов в сетевой
  *
  * @param
  * @en word
  * @ru word - 2-х байтное слово
  *
  * @return
  * @en Two bytes word with net byte order
  * @ru Двухбайтное слово с сетевым порядком байт
  */
quint16 host2net(quint16 word);

/**
  * @brief
  * @en Convert 2 bytes word from  net byte order to local byte order
  * @ru Конвертирует 16-битную беззнаковую величину из сетевого порядка байтов в локальный
  *
  * @param
  * @en word
  * @ru word - 2-х байтное слово
  *
  * @return
  * @en Two bytes word with local byte order
  * @ru Двухбайтное слово с локальным порядком байт
  */
quint16 net2host(quint16 word);

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

} // namespace modbus

#endif // UTILS_H
