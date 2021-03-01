################################################################
# modbus4qt library
# Copyright (C) 2012-2013, MELZ-INVEST JSC
# Author: Leonid Kolesnik
#
# This file based on Qwt Widget Library
# Copyright (C) 1997   Josef Wilgen
# Copyright (C) 2002   Uwe Rathmann
#
################################################################

MODBUS4QT_ROOT = $${PWD}/..
include($${MODBUS4QT_ROOT}/modbus4qt_config.pri)
include($${MODBUS4QT_ROOT}/modbus4qt_build.pri)
#include($${MODBUS4QT_ROOT}/modbus4qt_functions.pri)

TEMPLATE = app

QT += network widgets serialport

DESTDIR = $${MODBUS4QT_BUILD}/demo

LIBS += -lmodbus4qt
