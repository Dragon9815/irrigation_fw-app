/*****************************************************************************
 * @file        UART_Common.h
 * @brief       Stuff common to all UARTs
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __UART_COMMON_H__
#define __UART_COMMON_H__

#include "Common.h"
#include "mcu.h"
#include "utils/CircularQueue.h"

__SG_BEGIN_DECLS

#ifndef UART_TRANSMIT_BUFFER_SIZE
#    define UART_TRANSMIT_BUFFER_SIZE 2048
#endif

#ifndef UART_RECEIVE_BUFFER_SIZE
#    define UART_RECEIVE_BUFFER_SIZE 2048
#endif

typedef enum
{
    MyUART_TxMode_Interrupt,
    MyUART_TxMode_DMA
} MyUART_TxMode;

// typedef enum
//{
//	MyUART_RxMode_Blocking,
//	MyUART_RxMode_Interrupt,
//	MyUART_RxMode_DMA
//} MyUART_RxMode;

typedef struct
{
    USART_TypeDef * periph;

    CircularQueue_t txBuffer[2];
    CircularQueue_t rxBuffer;

    uint32_t LastSendT0;
    bool     FinishedTx;
    uint8_t  CurTxBuffer; // The index of the buffer currently used for storing
                          // bytes -> other buffer is/was transmitted

    bool IsIdle;

    struct
    {
        uint8_t TxBuffer[2][UART_TRANSMIT_BUFFER_SIZE];
        uint8_t RxBuffer[UART_RECEIVE_BUFFER_SIZE];
    } _;

    struct
    {
        bool           isRS485;
        GPIO_TypeDef * DEPort;
        uint32_t       DEPin;
    } RS485;
} MyUART_Context_t;

extern void MyUART_InitContext(MyUART_Context_t * context, USART_TypeDef * periph);
extern void MyUART_InitRS485(MyUART_Context_t * context, GPIO_TypeDef * DEPort, uint32_t DEPin);

extern bool   MyUART_TryGetByteRef(MyUART_Context_t * context, _OUT_ uint8_t * outByte);
extern size_t MyUART_GetBytesBlocking(MyUART_Context_t * context, _OUT_ uint8_t * buf, size_t count, uint32_t timeout);

extern void MyUART_SendByte(MyUART_Context_t * context, uint8_t b);
extern void MyUART_SendByteArray(MyUART_Context_t * context, const uint8_t * bytes, size_t byteCount);
extern void MyUART_SendString(MyUART_Context_t * context, const char * str);

extern void MyUART_FlushTx(MyUART_Context_t * context);
extern void MyUART_ExecuteThread(MyUART_Context_t * context);

extern void MyUART_IRQHandler(MyUART_Context_t * context);

__SG_END_DECLS

#endif /* __UART_COMMON_H__ */
