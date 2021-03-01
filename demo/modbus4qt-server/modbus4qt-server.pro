include( $${PWD}/../demo.pri )

TEMPLATE = app
TARGET = modbus4qt-server
MOC_DIR = $${MOC_DIR}/demo/server


SOURCES += \
    main.cpp \
    main_window.cpp

FORMS += \
    main_window.ui

HEADERS += \
    main_window.h
