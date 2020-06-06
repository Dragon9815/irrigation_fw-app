/*****************************************************************************
 * @file        Common.h
 * @brief       Things common to all source files
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __COMMON_H__
#define __COMMON_H__

//#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <types.h>
//typedef int8_t  i8;
//typedef int16_t i16;
//typedef int32_t i32;
//typedef int64_t i64;
//
//typedef uint8_t  u8;
//typedef uint16_t u16;
//typedef uint32_t u32;
//typedef uint64_t u64;

#if !defined(__SG_BEGIN_DECLS) || !defined(__WA_END_DECLS)
#    if __cplusplus > 0
#        define __SG_BEGIN_DECLS extern "C" {
#        define __SG_END_DECLS   }
#    else
#        define __SG_BEGIN_DECLS
#        define __SG_END_DECLS
#    endif /* __cplusplus > 0 */
#endif     /* !defined(__SG_BEGIN_DECLS) || !defined(__WA_END_DECLS) */

#ifndef _OUT_
#    define _OUT_
#endif /* _OUT_ */

#undef BIT
#define BIT(n) (1 << (n))

#define WA_UNUSED(x) (void)x

#ifdef __GNUC__
#    define ATTRIBUTE(a)  __attribute__((a))
#    define ALWAYS_INLINE ATTRIBUTE(always_inline)
#    define NAKED         ATTRIBUTE(naked)
#endif

#define IsBitSet(val, b) (((val) & (b)) == (b))
#define ClearBit(val, b) (val) &= ~(b)

__SG_BEGIN_DECLS

typedef enum wa_result
{
    WA_ERROR = -1,
    WA_OK    = 0,
} result_t;

__SG_END_DECLS

#endif /* __COMMON_H__ */
