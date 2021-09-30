/*****************************************************************************
 * modbus4qt Library
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 * Copyright (C) 2012-2015
 * http://www.modbus4qt.ru
 *
 * $Id: global.h 141 2015-09-15 07:21:06Z l.kolesnik $
 * $URL: https://o.m-i.ru/svn/modbus4qt/branches/1.1/src/global.h $
 *
 * $Id: global.h 141 2015-09-15 07:21:06Z l.kolesnik $
 * $URL: https://o.m-i.ru/svn/modbus4qt/branches/1.1/src/global.h $
 *
 *****************************************************************************/

/*****************************************************************************
* The contents of this file are subject to the Mozilla Public License Version 2.0
* (the "License"); you may not use this file except in compliance with the
* License. You may obtain a copy of the License at http://www.mozilla.org/MPL/
*
* Software distributed under the License is distributed on an "AS IS" basis,
* WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
* the specific language governing rights and limitations under the License.
*
* Alternatively, the contents of this file may be used under the terms of the
* GNU General Public License Version 2 or later (the "GPL"), in which case
* the provisions of the GPL are applicable instead of those above. If you wish to
* allow use of your version of this file only under the terms of the GPL and not
* to allow others to use your version of this file under the MPL, indicate your
* decision by deleting the provisions above and replace them with the notice and
* other provisions required by the GPL. If you do not delete the provisions
* above, a recipient may use your version of this file under either the MPL or
* the GPL.
*****************************************************************************/

#ifndef MODBUS4QT_GLOBAL_H
#define MODBUS4QT_GLOBAL_H

#include <QtCore/qglobal.h>

// MODBUS4QT_VERSION is (major << 16) + (minor << 8) + patch.

#define MODBUS4QT_VERSION       0x010000
#define MODBUS4QT_VERSION_STR   "1.0.0"

#if defined(Q_WS_WIN) || defined(Q_WS_S60)

    #if defined(_MSC_VER) /* MSVC Compiler */
    /* template-class specialization 'identifier' is already instantiated */
    #pragma warning(disable: 4660)
    #endif // _MSC_VER


    #ifdef MODBUS4QT_DLL

        #if defined(MODBUS4QT_MAKEDLL)	// create a modbus4qt dll library
            #define MODBUS4QT_EXPORT __declspec(dllexport)
            #define MODBUS4QT_TEMPLATEDLL
        #else							// use a modbus4qt dll library
            #define MODBUS4QT_EXPORT __declspec(dllimport)
        #endif

    #endif // MODBUS4QT_DLL

#else // Q_WS_WIN || Q_WS_S60

    #if defined(MODBUS4QT_MAKEDLL)
        #define MODBUS4QT_EXPORT Q_DECL_EXPORT
    #else
        #define MODBUS4QT_EXPORT Q_DECL_IMPORT
    #endif // MODBUS4QT_MAKEDLL
#endif

#endif // MODBUS4QT_GLOBAL_H
