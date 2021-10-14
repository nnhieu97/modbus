################################################################
#
# modbus4qt library
# Copyright (C) 2012-2021
# Author: Leonid Kolesnik
#
################################################################

MODBUS4QT_ROOT = $${PWD}
MODBUS4QT_BUILD = $${MODBUS4QT_ROOT}/build

include($${MODBUS4QT_ROOT}/modbus4qt_config.pri)
include($${MODBUS4QT_ROOT}/modbus4qt_build.pri)

QT += network widgets serialport

INCLUDEPATH += $${MODBUS4QT_ROOT}
LIBS += -L$${MODBUS4QT_BUILD}/lib -lmodbus4qt
