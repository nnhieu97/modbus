################################################################
#
# modbus4qt library
# Copyright (C) 2012-2021
# Author: Leonid Kolesnik
#
################################################################

MODBUS4QT_ROOT = $${PWD}
include($${MODBUS4QT_ROOT}/modbus4qt_config.pri)
include($${MODBUS4QT_ROOT}/modbus4qt_build.pri)

QT += network widgets serialport

DESTDIR = $${MODBUS4QT_BUILD}/demo

LIBS += -lmodbus4qt
