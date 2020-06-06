/*****************************************************************************
 * @file        UART_Common.c
 * @brief       Stuff common to all UARTs
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "dev/uart/UART_Common.h"

#include "mcu.h"
#include "system/tasking.h"
#include "system/timing.h"
#include "utils/memory.h"

#include <string.h>

static void RS485_SetPin(MyUART_Context_t * context)
{
    LL_GPIO_SetOutputPin(context->RS485.DEPort, context->RS485.DEPin);
}

static void RS485_ResetPin(MyUART_Context_t * context)
{
    LL_GPIO_ResetOutputPin(context->RS485.DEPort, context->RS485.DEPin);
}

static uint8_t getOtherBuffer(uint8_t currentBuffer)
{
    if(currentBuffer == 0)
        return 1;
    else
        return 0;
}

void MyUART_InitContext(MyUART_Context_t * context, USART_TypeDef * periph)
{
    FillZero(context, sizeof(context));
    context->periph     = periph;
    context->FinishedTx = true;
    //	context->IsIdle = true;

    CircQueue_CreateFromStack(&(context->rxBuffer), UART_RECEIVE_BUFFER_SIZE, context->_.RxBuffer);
    CircQueue_CreateFromStack(&(context->txBuffer[0]), UART_RECEIVE_BUFFER_SIZE, context->_.TxBuffer[0]);
    CircQueue_CreateFromStack(&(context->txBuffer[1]), UART_RECEIVE_BUFFER_SIZE, context->_.TxBuffer[1]);
}

void MyUART_InitRS485(MyUART_Context_t * context, GPIO_TypeDef * DEPort, uint32_t DEPin)
{
    context->RS485.isRS485 = true;
    context->RS485.DEPort  = DEPort;
    context->RS485.DEPin   = DEPin;

    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    GPIO_InitStruct.Pin                 = DEPin;
    GPIO_InitStruct.Mode                = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OutputType          = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Speed               = LL_GPIO_SPEED_FREQ_LOW;
    LL_GPIO_Init(DEPort, &GPIO_InitStruct);
    LL_GPIO_ResetOutputPin(DEPort, DEPin);
}

bool MyUART_TryGetByteRef(MyUART_Context_t * context, _OUT_ uint8_t * outByte)
{
    if(CircQueue_Count(&(context->rxBuffer)) > 0) {
        *outByte = CircQueue_DequeueByte(&(context->rxBuffer));
        return true;
    }
    return false;
}

size_t MyUART_GetBytesBlocking(MyUART_Context_t * context, _OUT_ uint8_t * buf, size_t count, uint32_t timeout)
{
    uint32_t T0, T1, dT;

    T0    = Timing_GetTicks_ms();
    dT    = 0;
    u32 i = 0;
    while(dT < timeout) {
        T1 = Timing_GetTicks_ms();
        dT = T1 - T0;

        if(CircQueue_Count(&(context->rxBuffer)) > 0) {
            buf[i] = CircQueue_DequeueByte(&(context->rxBuffer));
            i++;

            if(i >= count)
                break;
        }

        task_yield();
    }


    return i;
}

void MyUART_SendByte(MyUART_Context_t * context, uint8_t b)
{
    CircQueue_EnqueueByte(&(context->txBuffer[context->CurTxBuffer]), b);
}

void MyUART_SendByteArray(MyUART_Context_t * context, const uint8_t * bytes, size_t byteCount)
{
    for(size_t i = 0; i < byteCount; i++) {
        MyUART_SendByte(context, bytes[i]);
    }
}

void MyUART_SendString(MyUART_Context_t * context, const char * str)
{
    MyUART_SendByteArray(context, (uint8_t *)str, strlen(str));
}

void MyUART_ExecuteThread(MyUART_Context_t * context)
{
    uint32_t T1 = Timing_GetTicks_ms();
    uint32_t dT = T1 - context->LastSendT0;

    // Flush buffer every 1 ms or when buffer is half full
    if(dT > 1 || CircQueue_Count(&(context->txBuffer[context->CurTxBuffer])) > 512) {
        MyUART_FlushTx(context);
    }
}


void MyUART_FlushTx(MyUART_Context_t * context)
{
    if(!context->FinishedTx)
        return;

    if(CircQueue_Count(&(context->txBuffer[context->CurTxBuffer])) <= 0)
        return;

    context->FinishedTx  = false;
    context->LastSendT0  = Timing_GetTicks_ms();
    context->CurTxBuffer = getOtherBuffer(context->CurTxBuffer);
    CircQueue_Clear(&(context->txBuffer[context->CurTxBuffer]));

    LL_USART_EnableIT_TXE(context->periph);
}

void MyUART_IRQHandler(MyUART_Context_t * context)
{
    volatile uint32_t SR = context->periph->SR;

    if((SR & USART_SR_RXNE) != 0 && LL_USART_IsEnabledIT_RXNE(context->periph)) {
        //		context->IsIdle = false;
        LL_USART_ClearFlag_RXNE(context->periph);
        CircQueue_EnqueueByte(&(context->rxBuffer), LL_USART_ReceiveData8(context->periph));
    }

    if((SR & USART_SR_TXE) != 0 && LL_USART_IsEnabledIT_TXE(context->periph)) {
        //		if( context->IsIdle )
        //		{
        if(CircQueue_Count(&(context->txBuffer[getOtherBuffer(context->CurTxBuffer)])) > 0) {
            if(context->RS485.isRS485) {
                RS485_SetPin(context);
            }
            LL_USART_TransmitData8(context->periph,
                                   CircQueue_DequeueByte(&(context->txBuffer[getOtherBuffer(context->CurTxBuffer)])));
        }
        else {
            LL_USART_EnableIT_TC(context->periph);
            LL_USART_DisableIT_TXE(context->periph);
        }
        //		}
    }

    if((SR & USART_SR_TC) != 0 && LL_USART_IsEnabledIT_TC(context->periph)) {
        LL_USART_ClearFlag_TC(context->periph);
        if(context->RS485.isRS485) {
            RS485_ResetPin(context);
        }
        LL_USART_DisableIT_TC(context->periph);
        context->FinishedTx = true;
    }

    //	if( (SR & USART_SR_IDLE) != 0 && LL_USART_IsEnabledIT_IDLE(
    // context->periph ) )
    //	{
    //		LL_USART_ClearFlag_IDLE( context->periph );
    //		context->IsIdle = true;
    //	}
}
