/*****************************************************************************
 * @file        Timing.c
 * @brief       Timekeeping helper functions
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "system/timing.h"

#include "mcu.h"

void Timer_Reset(Timer_t * timer)
{
    timer->StartTicks = HAL_GetTick();
}

uint32_t Timer_GetDelta(Timer_t * timer)
{
    return HAL_GetTick() - timer->StartTicks;
}

struct
{
    uint32_t Ticks_ms;
    uint32_t Uptime_s;
    uint32_t LastUptimeTick;
} Timing = { 0 };

uint32_t Timing_GetTicks_ms(void)
{
    return Timing.Ticks_ms;
}

void Timing_Delay_ms(uint32_t ms)
{
    uint32_t T0 = Timing_GetTicks_ms();
    uint32_t T1, DeltaT = 0;

    // Add one tick, to ensure minimum delay length
    if(ms <= UINT32_MAX)
        ms += 1;

    while(DeltaT < ms) {
        T1     = HAL_GetTick();
        DeltaT = T1 - T0;
    }
}

uint32_t Timing_GetUptime(void)
{
    return Timing.Uptime_s;
}

void Timing_SysTickHandler(void)
{
    // SYSTICK runs at 1kHz -> 1 tick = 1 ms
    Timing.Ticks_ms++;
    while(Timing.Ticks_ms - Timing.LastUptimeTick >= 1000) {
        Timing.LastUptimeTick += 1000;
        Timing.Uptime_s++;
    }
}
