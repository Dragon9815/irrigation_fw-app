/*****************************************************************************
 * @file        serialDebug.c
 * @brief       Serial interface used for debugging
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "serial/serialDebug.h"

#include "config/dmaConfig.h"

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_dma.h>
#include <stm32f4xx_ll_usart.h>
#include <string.h>

#define SERIAL_DEBUG_UART USART2

#define SERIAL_DEBUG_TX_BUFFER_LENGTH 2048
#define SERIAL_DEBUG_RX_BUFFER_LENGTH 2048

static uint8_t  serialDebug_txBuffers[2][SERIAL_DEBUG_TX_BUFFER_LENGTH];
static uint32_t serialDebug_txBufferWriteIdx[2];
static uint8_t  serialDebug_txBufferNumber;
static bool     serialDebug_txBusy;

static uint8_t  serialDebug_rxBuffer[SERIAL_DEBUG_RX_BUFFER_LENGTH];
static uint32_t serialDebug_rxReadIdx;
static uint32_t serialDebug_rxWriteIdx;

/*****************************************************************************
 * @brief  Intialize the serial debug interface.
 * 
 * @author Stefan Grünzinger
 * @rev    1
 *****************************************************************************/
void serialDebug_initialize(void)
{
    LL_DMA_InitTypeDef DMA_InitStruct = {};

    serialDebug_txBufferNumber      = 0;
    serialDebug_txBufferWriteIdx[0] = 0;
    serialDebug_txBufferWriteIdx[1] = 0;

    // reset peripheral
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_USART2);
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_USART2);

    // enable peripheral clock
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

    // initialize USART
    LL_USART_InitTypeDef USART_InitStruct = {};
    USART_InitStruct.BaudRate             = 115200;
    USART_InitStruct.DataWidth            = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.HardwareFlowControl  = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling         = LL_USART_OVERSAMPLING_16;
    USART_InitStruct.Parity               = LL_USART_PARITY_NONE;
    USART_InitStruct.StopBits             = LL_USART_STOPBITS_1;
    USART_InitStruct.TransferDirection    = LL_USART_DIRECTION_TX_RX;
    LL_USART_Init(SERIAL_DEBUG_UART, &USART_InitStruct);

    LL_USART_ConfigAsyncMode(SERIAL_DEBUG_UART);
    LL_USART_EnableDMAReq_TX(SERIAL_DEBUG_UART);
    LL_USART_EnableIT_RXNE(SERIAL_DEBUG_UART);
    LL_USART_Enable(SERIAL_DEBUG_UART);

    NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(USART2_IRQn);

    // intialize TX DMA
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    DMA_InitStruct.Channel                = DMA_SERIALDEBUG_TX_CHANNEL_NR << DMA_SxCR_CHSEL_Pos;
    DMA_InitStruct.Direction              = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    DMA_InitStruct.Mode                   = LL_DMA_MODE_NORMAL;
    DMA_InitStruct.MemoryOrM2MDstAddress  = (uint32_t)serialDebug_txBuffers[0];
    DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
    DMA_InitStruct.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;
    DMA_InitStruct.PeriphOrM2MSrcAddress  = (u32) & (SERIAL_DEBUG_UART->DR);
    DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
    DMA_InitStruct.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;
    DMA_InitStruct.FIFOMode               = LL_DMA_FIFOMODE_DISABLE;
    DMA_InitStruct.MemBurst               = LL_DMA_MBURST_SINGLE;
    DMA_InitStruct.PeriphBurst            = LL_DMA_PBURST_SINGLE;
    DMA_InitStruct.NbData                 = SERIAL_DEBUG_RX_BUFFER_LENGTH;
    LL_DMA_Init(DMA_SERIALDEBUG_TX, DMA_SERIALDEBUG_TX_STREAM_NR, &DMA_InitStruct);

    LL_DMA_EnableIT_TC(DMA_SERIALDEBUG_TX, DMA_SERIALDEBUG_TX_STREAM_NR);

    NVIC_SetPriority(DMA1_Stream6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

static void serialDebug_checkAndStartDMA(void)
{
    u8 oldBufNumber;

    if(!serialDebug_txBusy && serialDebug_txBufferWriteIdx[serialDebug_txBufferNumber] > 0) {
        oldBufNumber               = serialDebug_txBufferNumber;
        serialDebug_txBufferNumber = (serialDebug_txBufferNumber ^ 1) & 1;
        serialDebug_txBusy         = true;
        LL_DMA_SetMemoryAddress(DMA_SERIALDEBUG_TX, DMA_SERIALDEBUG_TX_STREAM_NR,
                                (uint32_t)serialDebug_txBuffers[oldBufNumber]);
        LL_DMA_SetDataLength(DMA_SERIALDEBUG_TX, DMA_SERIALDEBUG_TX_STREAM_NR, serialDebug_txBufferWriteIdx[oldBufNumber]);
        LL_DMA_EnableStream(DMA_SERIALDEBUG_TX, DMA_SERIALDEBUG_TX_STREAM_NR);
    }
}

/*****************************************************************************
 * @brief  Send one single byte.
 * 
 * @param  byte The byte to send.
 *
 * @author Stefan Grünzinger
 * @rev    1
 *****************************************************************************/
void serialDebug_sendByte(u8 byte)
{
    serialDebug_sendBytes(&byte, 1);
}

/*****************************************************************************
 * @brief  Send an array of bytes.
 *
 * @param  bytes The array of bytes to send.
 * @param  count The amount of bytes to send.
 *
 * @author Stefan Grünzinger
 * @rev    1
 *****************************************************************************/
void serialDebug_sendBytes(const u8 * bytes, u32 count)
{
    int i;

    for(i = 0; i < count; i++) {
        if(serialDebug_txBufferWriteIdx[serialDebug_txBufferNumber] >= SERIAL_DEBUG_TX_BUFFER_LENGTH) {
            // buffer overflow
            break;
        }

        serialDebug_txBuffers[serialDebug_txBufferNumber][serialDebug_txBufferWriteIdx[serialDebug_txBufferNumber]] =
          bytes[i];
        serialDebug_txBufferWriteIdx[serialDebug_txBufferNumber]++;
    }

    serialDebug_checkAndStartDMA();
}

/*****************************************************************************
 * @brief  Sends a zero terminated string.
 * 
 * @param  str The zero terminated string to send.
 *
 * @author Stefan Grünzinger
 * @rev    1
 *****************************************************************************/
void serialDebug_sendString(const char * str)
{
    serialDebug_sendBytes((const u8 *)str, strlen(str));
}

/*****************************************************************************
 * @brief  Try to read an amount of bytes from the receive buffer.
 *
 * @param  buffer The buffer to store the read bytes in.
 * @param  count  The amount of bytes to read.
 *
 * @return The amount of bytes actually read, ranging from 0 to range.
 *
 * @author Stefan Grünzinger
 * @rev    1
 *****************************************************************************/
u32 serialDebug_readBytes(u8 * buffer, u32 count)
{
    uint32_t read = 0;

    while(serialDebug_rxReadIdx != serialDebug_rxWriteIdx && read < count) {
        buffer[read] = serialDebug_rxBuffer[serialDebug_rxReadIdx];
        serialDebug_rxReadIdx++;
        if(serialDebug_rxReadIdx >= SERIAL_DEBUG_RX_BUFFER_LENGTH) {
            serialDebug_rxReadIdx = 0;
        }
        read++;
    }

    return read;
}

void USART2_IRQHandler(void)
{
    if(LL_USART_IsActiveFlag_RXNE(SERIAL_DEBUG_UART)) {
        serialDebug_rxBuffer[serialDebug_rxWriteIdx] = LL_USART_ReceiveData8(SERIAL_DEBUG_UART);
        serialDebug_rxWriteIdx++;
        if(serialDebug_rxWriteIdx >= SERIAL_DEBUG_TX_BUFFER_LENGTH) {
            serialDebug_rxWriteIdx = 0;
        }
    }
}

void DMA1_Stream6_IRQHandler(void)
{
    u8 bufNumber;

    if(LL_DMA_IsActiveFlag_TC6(DMA_SERIALDEBUG_TX)) {
        LL_DMA_ClearFlag_TC6(DMA_SERIALDEBUG_TX);

        bufNumber                               = (serialDebug_txBufferNumber ^ 1) & 1;
        serialDebug_txBufferWriteIdx[bufNumber] = 0;
        serialDebug_txBusy                      = false;
        serialDebug_checkAndStartDMA();
    }
}
