#------------------------------------------------------------------------------
#  modbus4qt Library
#  Author: Leonid Kolesnik, l.kolesnik@m-i.ru
#  Copyright (C) 2012-2015
#  http://www.modbus4qt.ru
#
#  $Id: modbus4qt_functions.pri 147 2015-09-30 12:50:42Z l.kolesnik $
#  $URL: https://o.m-i.ru/svn/modbus4qt/branches/1.1/modbus4qt_functions.pri $
#------------------------------------------------------------------------------

################################################################
# Qwt Widget Library
# Copyright (C) 1997   Josef Wilgen
# Copyright (C) 2002   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
################################################################

# Copied and modified from qt_functions.prf and qwtfunctions.prf

defineReplace(modbus4qtLibraryTarget) {

    unset(LIBRARY_NAME)
    LIBRARY_NAME = $$1

    # Building in mac has not been tested as I do not have got it.
    #
    # It is an example from Qwt library files

    mac:contains(MODBUS4QT_CONFIG, Framework) {
        QMAKE_FRAMEWORK_BUNDLE_NAME = $$LIBRARY_NAME
        export(QMAKE_FRAMEWORK_BUNDLE_NAME)
    }

    contains(TEMPLATE, .*lib):CONFIG(debug, debug|release) {

        !debug_and_release|build_pass {

            mac:RET = $$member(LIBRARY_NAME, 0)_debug
            win32:RET = $$member(LIBRARY_NAME, 0)d
        }
    }

    isEmpty(RET):RET = $$LIBRARY_NAME
    return($$RET)
}

defineTest(modbus4qtAddLibrary) {

    LIB_PATH = $$1
    LIB_NAME = $$2

# we use Qt5 and higher
#
# Delete commented lines after testing
#
#    unix:lessThan(QT_MAJOR_VERSION, 5) {

        # Many Linux distributions install Qwt in the same directory
        # as the Qt libs and thus we need to prepend the path for the local build
        # to avoid conflicting with the installed version.
        # Qt5 qmake appends ( instead of prepending ) the path to the Qt libs
        # to LIBS, but for Qt4 we need to use the QMAKE_LIBDIR_FLAGS.

#        QMAKE_LIBDIR_FLAGS *= -L$${LIB_PATH}
#    }
#    else {
        LIBS *= -L$${LIB_PATH}
#    }

    unset(LINKAGE)

    isEmpty(LINKAGE) {

        if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {

            mac:LINKAGE = -l$${LIB_NAME}_debug
            win32:LINKAGE = -l$${LIB_NAME}d
            unix:LINKAGE = -l$${LIB_NAME}d
        }
    }

    isEmpty(LINKAGE) {

        LINKAGE = -l$${LIB_NAME}
    }

    !isEmpty(QMAKE_LSB) {

        QMAKE_LFLAGS *= --lsb-shared-libs=$${LIB_NAME}
    }

    LIBS += $$LINKAGE
    export(LIBS)
    export(QMAKE_LFLAGS)
    export(QMAKE_LIBDIR_FLAGS)

    return(true)
}
