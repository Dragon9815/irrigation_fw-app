/*****************************************************************************
 * @file        UART_Debug.h
 * @brief       UART for debugging purposes
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "dev/uart/UART_Config.h"

#include "config/GPIOConfig.h"
#include "system/tasking.h"

MyUART_Context_t UART_Config_Context;

void UART_Config_Task(void * ctx);

void UART_Config_Initialize(void)
{
    LL_USART_InitTypeDef USART_InitStruct = { 0 };
    LL_GPIO_InitTypeDef  GPIO_InitStruct  = { 0 };

    // reset peripheral
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_USART2);
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_USART2);

    // enable peripheral clock
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

    // setup gpios
    GPIO_InitStruct.Pin       = Config_UART_TX_Pin;
    GPIO_InitStruct.Mode      = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed     = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull      = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = Config_UART_TX_AF;
    LL_GPIO_Init(Config_UART_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = Config_UART_RX_Pin;
    GPIO_InitStruct.Mode      = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed     = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull      = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = Config_UART_RX_AF;
    LL_GPIO_Init(Config_UART_RX_GPIO_Port, &GPIO_InitStruct);

    // enable interrupts
    NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(USART2_IRQn);

    // initialize peripheral
    USART_InitStruct.BaudRate            = 115200;
    USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity              = LL_USART_PARITY_NONE;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling        = LL_USART_OVERSAMPLING_16;
    USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
    LL_USART_Init(USART2, &USART_InitStruct);
    LL_USART_ConfigAsyncMode(USART2);

    LL_USART_EnableIT_RXNE(USART2);

    MyUART_InitContext(&UART_Config_Context, USART2);

    LL_USART_Enable(USART2);

    task_add(UART_Config_Task, NULL, 512);
}

void UART_Config_Task(void * ctx)
{
    DEFAULT_TASK(UART_Config);
}

void UART_Config_ExecuteThread(void)
{
    MyUART_ExecuteThread(&UART_Config_Context);
}

//void USART2_IRQHandler( void )
//{
//    MyUART_IRQHandler( &UART_Config_Context );
//}
