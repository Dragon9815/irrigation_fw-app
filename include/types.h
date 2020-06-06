/*****************************************************************************
 * @file        types.h
 * @brief       Basic type definitions
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __TYPES_H__
#define __TYPES_H__

typedef char      i8;
typedef short     i16;
typedef long      i32;
typedef long long i64;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned long      u32;
typedef unsigned long long u64;

typedef _Bool bool;
#define true 1
#define false 0

#ifndef NULL
#    define NULL (void *)0
#endif

#endif /* __TYPES_H__ */
