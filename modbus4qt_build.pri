#------------------------------------------------------------------------------
#  modbus4qt Library
#  Author: Leonid Kolesnik, l.kolesnik@m-i.ru
#  Copyright (C) 2012-2021
#  https://mt11.net.ru
#------------------------------------------------------------------------------
#
# This library is free software.
#
# The contents of this file are subject to the Mozilla Public License Version 1.1
# (the "License"); you may not use this file except in compliance with the
# License. You may obtain a copy of the License at http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
# the specific language governing rights and limitations under the License.
#
# Alternatively, the contents of this file may be used under the terms of the
# GNU General Public License Version 2 or later (the "GPL"), in which case
# the provisions of the GPL are applicable instead of those above. If you wish to
# allow use of your version of this file only under the terms of the GPL and not
# to allow others to use your version of this file under the MPL, indicate your
# decision by deleting the provisions above and replace them with the notice and
# other provisions required by the GPL. If you do not delete the provisions
# above, a recipient may use your version of this file under either the MPL or
# the GPL.
#
#------------------------------------------------------------------------------

######################################################################
# qmake internal options
######################################################################

CONFIG += qt
CONFIG += warn_on
CONFIG += silent

QT += serialport

######################################################################
# release/debug mode
######################################################################

win32 {

    # On Windows you can't mix release and debug libraries.
    # The designer is built in release mode. If you like to use it
    # you need a release version. For your own application development you
    # might need a debug version.
    # Enable debug_and_release + build_all if you want to build both.

    CONFIG           += debug_and_release
    CONFIG           += build_all
}
else {

    # Use debug or release config according you needs
    #
    # CONFIG           += release
    # CONFIG           += debug

    CONFIG           += debug

    VER_MAJ           = $${MODBUS4QT_VER_MAJ}
    VER_MIN           = $${MODBUS4QT_VER_MIN}
    VER_PAT           = $${MODBUS4QT_VER_PAT}
    VERSION           = $${MODBUS4QT_VERSION}
}

linux-g++ | linux-g++-64 {
    # CONFIG           += separate_debug_info
}

CONFIG(release){
    DEFINES += QT_NO_DEBUG QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
}

CONFIG(debug){
    DEFINES -= QT_NO_DEBUG QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
}

######################################################################
# paths for building modbus4qt
######################################################################

MODBUS4QT_ROOT  = $${PWD}
MODBUS4QT_BUILD = $${MODBUS4QT_ROOT}/build

DESTDIR         = $${MODBUS4QT_BUILD}/bin
MOC_DIR         = $${MODBUS4QT_BUILD}/moc
OBJECTS_DIR     = $${MODBUS4QT_BUILD}/obj
UI_DIR          = $${MODBUS4QT_BUILD}/ui
RCC_DIR         = $${MODBUS4QT_BUILD}/resources
QMAKE_RPATHDIR *= $${MODBUS4QT_BUILD}/lib

INCLUDEPATH += $${MODBUS4QT_ROOT}/src
DEPENDPATH  += $${MODBUS4QT_ROOT}/src
LIBS           += -L$${MODBUS4QT_BUILD}/lib
