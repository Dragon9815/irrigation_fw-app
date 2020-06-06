/*****************************************************************************
 * @file        Timing.h
 * @brief       Timekeeping helper functions
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __TIMING_H__
#define __TIMING_H__

#include "Common.h"

__SG_BEGIN_DECLS

// ----------------------------------------------------------------------------
//  High level timer
// ----------------------------------------------------------------------------
typedef struct
{
    uint32_t StartTicks;
} Timer_t;

extern void     Timer_Reset(Timer_t * timer);
extern uint32_t Timer_GetDelta(Timer_t * timer);

// ----------------------------------------------------------------------------
//  Low level timing
// ----------------------------------------------------------------------------
extern uint32_t Timing_GetTicks_ms(void);
extern void     Timing_Delay_ms(uint32_t ms);
extern uint32_t Timing_GetUptime(void);

extern void Timing_SysTickHandler(void);

__SG_END_DECLS

#endif /* __TIMING_H__ */
