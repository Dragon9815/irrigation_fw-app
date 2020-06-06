/*****************************************************************************
 * @file        Debug.c
 * @brief       Functions for debugging
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "Common.h"
#include "mcu.h"

#include <dev/uart/UART_Config.h>

int __io_putchar(int ch)
{
    MyUART_SendByte(&UART_Config_Context, (uint8_t)ch);
    // ITM_SendChar( (char)ch );
    return 1;
}
