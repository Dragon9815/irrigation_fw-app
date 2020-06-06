/*****************************************************************************
 * @file        syscall.h
 * @brief       Syscall (SVC) handling
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "system/syscall.h"

#include "stm32f4xx.h"

static syscall_func registered_syscalls[MAX_SYSCALLS] = { 0 };

void syscall_handler(u32 * svc_data)
{
    u8           svc_number = ((u8 *)svc_data[6])[-2];
    syscall_func handler    = registered_syscalls[svc_number];
    if(handler == NULL) {
        // invalid syscall !!
        while(true) {
        }
    }
    __disable_irq();
    handler(svc_data);
    __enable_irq();

    // set pendsv as pending
    SCB->ICSR |= (1 << SCB_ICSR_PENDSVSET_Pos);
}

result_t syscall_register(u8 num, syscall_func handler)
{
    registered_syscalls[num] = handler;
    return WA_OK;
}

NAKED void SVC_Handler(void)
{
    // save the interrupt return pointer
    asm volatile("push {lr}\n\t");

    // save psp to r0, which is the first parameter when calling the C function
    asm volatile("mrs r0, psp\n\t");

    // save task context to stack
    asm volatile("mrs r12, psp\n\t");
    asm volatile("stmdb r12!,{r4-r11}\n\t");
    asm volatile("msr psp,r12\n\t");

    // call syscall handler
    asm volatile("bl syscall_handler\n\t");

    // return from interrupt
    asm volatile("pop {lr}\n\t");
    asm volatile("bx lr\n\t");
}

NAKED void PendSV_Handler(void)
{
    // restore task context from stack
    asm volatile("mrs r12, psp\n\t");
    asm volatile("ldmfd r12!,{r4-r11}\n\t");
    asm volatile("msr psp, r12\n\t");

    // return from interrupt
    asm volatile("bx lr\n\t");
}
