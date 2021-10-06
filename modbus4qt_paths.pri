#------------------------------------------------------------------------------
#  modbus4qt Library
#  Author: Leonid Kolesnik, l.kolesnik@m-i.ru
#  Copyright (C) 2012-2021
#  https://mt11.net.ru
#------------------------------------------------------------------------------
#

######################################################################
# paths for building modbus4qt
######################################################################

MODBUS4QT_ROOT  = $${PWD}
MODBUS4QT_BUILD = $${MODBUS4QT_ROOT}/build

DESTDIR         = $${MODBUS4QT_BUILD}/bin
MOC_DIR         = $${MODBUS4QT_BUILD}/moc
OBJECTS_DIR     = $${MODBUS4QT_BUILD}/obj
UI_DIR          = $${MODBUS4QT_BUILD}/ui
RCC_DIR         = $${MODBUS4QT_BUILD}/rc
QMAKE_RPATHDIR *= $${MODBUS4QT_BUILD}/lib

INCLUDEPATH += $${MODBUS4QT_ROOT}

DEPENDPATH  += $${MODBUS4QT_ROOT}/modbus4qt
