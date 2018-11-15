#------------------------------------------------------------------------------
#  modbus4qt Library
#  Author: Leonid Kolesnik, l.kolesnik@m-i.ru
#  Copyright (C) 2012-2015
#  http://www.modbus4qt.ru
#
#  $Id: modbus4qt_build.pri 138 2015-09-12 17:00:04Z l.kolesnik $
#  $URL: https://o.m-i.ru/svn/modbus4qt/branches/1.1/modbus4qt_build.pri $
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

CONFIG           += qt
CONFIG           += warn_on
CONFIG           += silent

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += serialport
}
else {
    CONFIG += serialport
}

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

    #CONFIG           += release
    CONFIG           += debug

    VER_MAJ           = $${MODBUS4QT_VER_MAJ}
    VER_MIN           = $${MODBUS4QT_VER_MIN}
    VER_PAT           = $${MODBUS4QT_VER_PAT}
    VERSION           = $${MODBUS4QT_VERSION}
}

linux-g++ {
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

MOC_DIR      = moc
RCC_DIR      = resources
!debug_and_release {
    OBJECTS_DIR       = obj
}

unix {

    exists( $${QMAKE_LIBDIR_QT}/libmodbus4qt.* ) {

        # On some Linux distributions the libraries are installed
        # in the same directory as the Qt libraries. Unfortunately
        # qmake always adds QMAKE_LIBDIR_QT at the beginning of the
        # linker path, so that the installed libraries will be
        # used instead of the local ones.

        error( "local build will conflict with $${QMAKE_LIBDIR_QT}/libmodbus4qt.*" )
    }
}

OTHER_FILES += \
    ../doc/modbus-ap-v1_1.dox \
    ../doc/modbus4qt.dox
