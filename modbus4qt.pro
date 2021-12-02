#-------------------------------------------------
#  modbus4qt Library
#  Author: Leonid Kolesnik, l.kolesnik@m-i.ru
#  Copyright (C) 2012-2021
#  https://mt11.net.ru/elvactech-lab/modbus4qt
#-------------------------------------------------
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
#-------------------------------------------------

include (modbus4qt_config.pri)

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
    modbus4qt

contains (MODBUS4QT_CONFIG, modbus4qt_demo) {
    SUBDIRS += demo
}

#modbus4qtspec.files  = modbus4qt_config.pri modbus4qt_functions.pri modbus4qt.prf
#modbus4qtspec.path  = $${MODBUS4QT_INSTALL_FEATURES}

#INSTALLS += modbus4qtspec
