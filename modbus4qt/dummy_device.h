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

#ifndef DUMMYDEVICE_H
#define DUMMYDEVICE_H

#include "device.h"

#include <QVector>

namespace modbus4qt
{

/**
 * @brief
 * @en Class for dummy device which does not use any external equipment and only store coils and registers in memory
 * @ru Класс для устройства-заглушки, которое не использует никакого реального оборудования, а хранит все данные в памяти
 *
 * @en Class is intented for testing purposes
 * @ru Класс предназначен для тестирования
 */
class DummyDevice : public Device
{
    Q_OBJECT

    private:

        /**
         * @brief
         * @en Array for coils
         * @ru Массив для хранения значений дискретных выходов
         */
        QVector<bool> coils_;

        /**
         * @brief
         * @en Array for descrete inputs
         * @ru Массив для хранения значений дискретных входов
         */
        QVector<bool> discreteInputs_;

        /**
         * @brief
         * @en Array for input registers
         * @ru Массив для хранения значений регистров ввода
         */
        QVector<quint16> holdingRegisters_;

        /**
         * @brief
         * @en Array for holding registers
         * @ru Массив для храненения значений регистров вывода
         */
        QVector<quint16> inputRegisters_;

    public:
        DummyDevice();
};

} // namespace modbus4qt

#endif // DUMMYDEVICE_H
