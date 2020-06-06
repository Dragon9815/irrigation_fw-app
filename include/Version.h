/*****************************************************************************
 * @file        Version.h
 * @brief       Version information
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __VERSION_H__
#define __VERSION_H__

#include "Common.h"

#ifndef FW_VERSION_MAJOR
#    define FW_VERSION_MAJOR 99
#endif

#ifndef FW_VERSION_MINOR
#    define FW_VERSION_MINOR 99
#endif

#ifndef FW_VERSION_SUBMINOR
#    define FW_VERSION_SUBMINOR 99
#endif

#define BL_VERSION_MAJOR    -1
#define BL_VERSION_MINOR    -1
#define BL_VERSION_SUBMINOR -1

#ifndef HW_VERSION
#    error "No HW Version defined!"
#endif

#endif /* __VERSION_H__ */
