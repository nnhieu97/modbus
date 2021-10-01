################################################################
#
# modbus4qt library
# Copyright (C) 2012-2013, MELZ-INVEST JSC
# Author: Leonid Kolesnik
#
################################################################

#MODBUS4QT_ROOT = $${PWD}/../

#include( $${MODBUS4QT_ROOT}/modbus4qt_config.pri )

TEMPLATE = subdirs

    SUBDIRS += \
        master-rtu \
        master-tcp
        #modbus4qt-server
