/*****************************************************************************
 * @file        SystemISR.c
 * @brief       Cortex M4 interrupt handlers
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "mcu.h"
#include "system/timing.h"

/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{

    while(1) {
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{

    while(1) {
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
    while(1) {
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
    while(1) {
    }
}

// volatile int _svc_test_var = 0;
//
///**
//  * @brief This function handles System service call via SWI instruction.
//  */
// void SVC_Handler(void)
//{
//	_svc_test_var ++;
//}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{}

/**
 * @brief This function handles Pendable request for system service.
 */
//__attribute__((naked))
// void PendSV_Handler(void)
//{
//	asm volatile ("bx lr");
//}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
    Timing_SysTickHandler();
    HAL_IncTick();
}
