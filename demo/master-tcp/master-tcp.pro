include( $${PWD}/../demo.pri )

TARGET = master-tcp

MOC_DIR = $${MOC_DIR}/demo/master-tcp
OBJECTS_DIR = $${OBJECTS_DIR}/demo/master-tcp

SOURCES += \
    main.cpp\
    main_window.cpp

HEADERS  += \
    main_window.h \
    main.h

