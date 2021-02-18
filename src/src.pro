#------------------------------------------------------------------------------
#
# modbus4qt Library
# Copyright (C) 2012 MELZ-Invest JSC
# http://www.m-i.ru/projects/modbus4qt
# Author: Leonid Kolesnik, l.kolesnik@m-i.ru
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

MODBUS4QT_ROOT = $${PWD}/..
include( $${MODBUS4QT_ROOT}/modbus4qt_config.pri )
include( $${MODBUS4QT_ROOT}/modbus4qt_build.pri )

TEMPLATE = lib

#win32: {
    TARGET = $$qtLibraryTarget(modbus4qt)
#    TARGET = $$modbus4qtLibraryTarget(modbus4qt)
#}
#else {
#    CONFIG(debug, debug|release) {
#        TARGET = modbus4qtd
#    }
#    else {
#        TARGET = modbus4qt
#    }
#}

DESTDIR = $${MODBUS4QT_ROOT}/lib

contains(MODBUS4QT_CONFIG, modbus4qt_dll) {
    CONFIG += dll
    win32: DEFINES += QT_DLL MODBUS4QT_DLL MODBUS4QT_MAKEDLL
}
else {
    CONFIG += staticlib
}

SOURCES += utils.cpp \
    tcp_client.cpp \
    consts.cpp \
    client.cpp \
    rtu_client.cpp \
    server.cpp \
#    tcp_server.cpp \
    rtu_server.cpp \
    tcp_server.cpp \
    device.cpp \
    dummy_device.cpp

HEADERS += global.h \
    consts.h \
    types.h \
    utils.h \
    tcp_client.h \
    client.h \
    rtu_client.h \
    server.h \
#    tcp_server.h \
    rtu_server.h \
    tcp_server.h \
    device.h \
    dummy_device.h

# Install directives

target.path    = $${MODBUS4QT_INSTALL_LIBS}

doc.files      = $${MODBUS4QT_ROOT}/doc/html
unix:doc.files += $${MODBUS4QT_ROOT}/doc/man
doc.path       = $${MODBUS4QT_INSTALL_DOCS}

INSTALLS       = target doc

headers.files  = $${HEADERS}
headers.path   = $${MODBUS4QT_INSTALL_HEADERS}
INSTALLS += headers


