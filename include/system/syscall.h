/*****************************************************************************
 * @file        syscall.h
 * @brief       Syscall (SVC) handling
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __SYSTEM_SYSCALL_H__
#define __SYSTEM_SYSCALL_H__

#include "Common.h"

#define SYSCALL(code) asm volatile("svc %0\n\t" : : "I"(code))
#define MAX_SYSCALLS  256

#define SYSCALL_YIELD 0x00
#define SYSCALL_EXIT  0x01

__SG_BEGIN_DECLS

typedef void (*syscall_func)(u32 * svc_data);

extern result_t syscall_register(u8 num, syscall_func handler);

__SG_END_DECLS

#endif /* __SYSTEM_SYSCALL_H__ */
