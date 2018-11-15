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
include($${MODBUS4QT_ROOT}/modbus4qt_functions.pri)

MODBUS4QT_OUT_ROOT = $${OUT_PWD}/../..

TEMPLATE = app

QT += network

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets serialport
}
else {
    CONFIG += serialport
}

INCLUDEPATH += $${MODBUS4QT_ROOT}/src
DEPENDPATH  += $${MODBUS4QT_ROOT}/src
QMAKE_RPATHDIR += $${MODBUS4QT_ROOT}/lib

!debug_and_release {
    DESTDIR      = $${MODBUS4QT_ROOT}/demo/bin
}
else {
    CONFIG(debug, debug|release) {
        DESTDIR = $${MODBUS4QT_ROOT}/demo/bin_debug
    }
    else {
        DESTDIR = $${MODBUS4QT_ROOT}/demo/bin
    }
}

modbus4qtAddLibrary($${MODBUS4QT_ROOT}/lib, modbus4qt)
