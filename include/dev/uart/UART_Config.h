/*****************************************************************************
 * @file        UART_Config.h
 * @brief       UART for debugging purposes
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __UART_DEBUG_H__
#    define __UART_CONFIG_H__

#    include "dev/uart/UART_Common.h"

__SG_BEGIN_DECLS

extern MyUART_Context_t UART_Config_Context;

extern void UART_Config_Initialize(void);
extern void UART_Config_ExecuteThread(void);

__SG_END_DECLS

#endif /* __UART_DEBUG_H__ */
