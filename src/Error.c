/*****************************************************************************
 * @file        Error.c
 * @brief       Handling of errors
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "Common.h"
#include "mcu.h"

#include <stdio.h>

void Error_Handler(void)
{
    volatile int i = 0;
    for(;;) {
        __BKPT(0);
        i++; // Debugger breakpoint
    }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t * file, uint32_t line)
{
    (void)file;
    (void)line;
    volatile int i = 0;
    for(;;) {
        __BKPT(0);
        i++; // Debugger breakpoint
    }
}

#endif /* USE_FULL_ASSERT */
