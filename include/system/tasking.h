/*****************************************************************************
 * @file        tasking.h
 * @brief       Multitasking implementation
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __SYSTEM_TASKING_H__
#define __SYSTEM_TASKING_H__

#include "Common.h"

#define OS_MAX_TASKS       19
#define OS_TASK_STACK_SIZE 2048

#define DEFAULT_TASK(name)      \
    (void)ctx;                  \
    while(true) {               \
        name##_ExecuteThread(); \
        task_yield();           \
    }

__SG_BEGIN_DECLS

typedef u32 taskid_t;

typedef void (*osTaskFunc_t)(void * context);

extern void                           tasking_init(void);
__attribute__((noreturn)) extern void tasking_start(void);

extern taskid_t task_add(osTaskFunc_t entryPoint, void * context, u32 stack_size);
extern bool     task_is_running(taskid_t pid);

extern void task_yield(void);
extern void task_delay(uint32_t ms);

__SG_END_DECLS

#endif /* __SYSTEM_TASKING_H__ */
