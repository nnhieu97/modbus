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

include( $${PWD}/../modbus4qt_config.pri )

TEMPLATE = subdirs

    SUBDIRS += \
        master-tcp \
        master-rtu \
        modbus4qt-server
