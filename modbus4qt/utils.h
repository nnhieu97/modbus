/*****************************************************************************
 * modbus4qt Library
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 * Copyright (C) 2012-2015
 * http://www.modbus4qt.ru
 *
 * $Id: utils.h 130 2015-09-08 11:50:06Z l.kolesnik $
 * $URL: https://o.m-i.ru/svn/modbus4qt/branches/1.1/src/utils.h $
 *****************************************************************************/

/*****************************************************************************
* The contents of this file are subject to the Mozilla Public License Version 2.0
* (the "License"); you may not use this file except in compliance with the
* License. You may obtain a copy of the License at http://www.mozilla.org/MPL/
*
* Software distributed under the License is distributed on an "AS IS" basis,
* WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
* the specific language governing rights and limitations under the License.
*
* Alternatively, the contents of this file may be used under the terms of the
* GNU General Public License Version 2 or later (the "GPL"), in which case
* the provisions of the GPL are applicable instead of those above. If you wish to
* allow use of your version of this file only under the terms of the GPL and not
* to allow others to use your version of this file under the MPL, indicate your
* decision by deleting the provisions above and replace them with the notice and
* other provisions required by the GPL. If you do not delete the provisions
* above, a recipient may use your version of this file under either the MPL or
* the GPL.
*****************************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include <QByteArray>
#include <QVector>
#include <QMutex>
#include <QWaitCondition>

namespace modbus4qt
{

/**
 * @brief
 * @en Calculate the checksum of the data buffer for the crc16 algorithm
 * @ru Расчитывает контрольную сумму буфера данных по алгоритму crc16
 *
 * @param
 * @en buf - buffer to calculate crc
 * @ru buf - буфер, для которого производится расчет
 *
 * @return
 * @en CRC calculated
 * @ru расчитанная контрольная сумма
 *
 * @en The code of this function was taken from из <a href="http://www.libmodbus.org">libmodbus</a>
 * @ru Код этой функции заимствован из <a href="http://www.libmodbus.org">libmodbus</a>
 */
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
inline quint8 hi(quint16 word)
{
    quint8* bytes = (quint8*)&word;

    quint16 test = 1; // * 0x0001 *
    if (*((quint8*)&test) == 0)
        return bytes[0];	// big endian (network/motorolla)
    else
        return bytes[1];	// little endian (intel)
}

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
inline quint8 lo(quint16 word)
{
    quint8* bytes = (quint8*)&word;

    quint16 test = 1; // * 0x0001 *
    if (*((quint8*)&test) == 0)
        return bytes[1];	// big endian (network/motorolla)
    else
        return bytes[0];	// little endian (intel)
}

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
inline quint16 swap(quint16 word)
{
    quint16 result = lo(word);
    result = result << 8;
    result += hi(word);

    return result;
}

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
inline quint16 host2net(quint16 word)
{
    quint16 test = 1; // * 0x0001 *
    if (*((quint8*)&test) == 0)
        return word;		// big endian (network/motorolla)
    else
        return swap(word);	// little endian (intel)
}

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
inline quint16 net2host(quint16 word)
{
    quint16 test = 1; // * 0x0001 *
    if (*((quint8*)&test) == 0)
        return word;		// big endian (network/motorolla)
    else
        return swap(word);	// little endian (intel)
}

/**
  * @brief
  * @en Wait time milliseconds
  * @ru Сделать паузу в выполнении
  *
  * @param
  * @en time - pause time, milliseconds
  * @ru time - время задержки в миллисекундах
  */
inline void wait(int time)
{
    QMutex mutex;
    mutex.lock();

    QWaitCondition pause;
    pause.wait(&mutex, time);
}

} // namespace modbus

#endif // UTILS_H
