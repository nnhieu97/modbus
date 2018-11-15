/*****************************************************************************
 * modbus4qt Library
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 * Copyright (C) 2012-2015
 * http://www.modbus4qt.ru
 *
 * $Id: dummy_device.h 173 2016-04-27 17:22:31Z l.kolesnik $
 * $URL: https://o.m-i.ru/svn/modbus4qt/branches/1.1/src/dummy_device.h $
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
