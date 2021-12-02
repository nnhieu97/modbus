MODBUS4QT_ROOT = $${PWD}/../../

include( $${MODBUS4QT_ROOT}/demo/demo.pri )

TEMPLATE = app

TARGET = master-rtu

MOC_DIR = $${MOC_DIR}/demo/master-rtu
OBJECTS_DIR = $${OBJECTS_DIR}/demo/master-rtu

SOURCES += \
    main.cpp \
    main_window.cpp \

HEADERS  += \
    main_window.h

#target.path = $${MODBUS4QT_INSTALL_BIN}

#
INSTALLS = target
