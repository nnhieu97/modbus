#------------------------------------------------------------------------------
#  modbus4qt Library
#  Author: Leonid Kolesnik, l.kolesnik@m-i.ru
#  Copyright (C) 2012-2021
#  https://mt11.net.ru
#------------------------------------------------------------------------------
#
# This file is part of modbus4qt.
#
# modbus4qt is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# modbus4qt is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# modbus4qt.
#
# If not, see <https://www.gnu.org/licenses/>.
#
#------------------------------------------------------------------------------

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
