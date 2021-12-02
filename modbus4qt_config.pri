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

#MODBUS4QT_VER_MAJ = 1
#MODBUS4QT_VER_MIN = 0
#MODBUS4QT_VER_PAT = 0
#MODBUS4QT_VERSION = $${MODBUS4QT_VER_MAJ}.$${MODBUS4QT_VER_MIN}.$${MODBUS4QT_VER_PAT}

#------------------------------------------------------------------------------
# Install paths
#------------------------------------------------------------------------------

#MODBUS4QT_INSTALL_PREFIX = $$[QT_INSTALL_PREFIX]

#unix {
#    MODBUS4QT_INSTALL_PREFIX    = /usr/local
#}

#win32 {
#    MODBUS4QT_INSTALL_PREFIX = c:/lib/modbus4qt-$$MODBUS4QT_VERSION
#}

#MODBUS4QT_INSTALL_BIN       = $${MODBUS4QT_INSTALL_PREFIX}/bin
#MODBUS4QT_INSTALL_DOCS      = $${MODBUS4QT_INSTALL_PREFIX}/doc/modbus4qt
#MODBUS4QT_INSTALL_HEADERS   = $${MODBUS4QT_INSTALL_PREFIX}/include/modbus4qt
#MODBUS4QT_INSTALL_LIBS      = $${MODBUS4QT_INSTALL_PREFIX}/lib

#------------------------------------------------------------------------------
# Features
#
# When building modbus4qt application with qmake you might want to load
# the compiler/linker flags, that are required to build a modbus4qt application
# from modbus4qt.prf. Therefore all you need to do is to add "CONFIG += modbus4qt"
# to your project file and take care, that modbus4qt.prf can be found by qmake.
#
# I recommend not to install the modbus4qt features together with the
# Qt features, because you will have to reinstall the modbus4qt features,
# with every Qt upgrade.

#MODBUS4QT_INSTALL_FEATURES  = $${MODBUS4QT_INSTALL_PREFIX}/features

#------------------------------------------------------------------------------
# Build the static/shared libraries
#
# If dll is enabled, a shared library is built, otherwise
# it will be a static library.

MODBUS4QT_CONFIG += modbus4qt_dll

#------------------------------------------------------------------------------
# Build demo suite
#
# If you want to auto build the demo suite, enable the line below
# Otherwise you have to build them from the examples directory.

MODBUS4QT_CONFIG     += modbus4qt_demo
