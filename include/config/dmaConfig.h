/*****************************************************************************
 * @file        dmaDefs.h
 * @brief       DMA definitions
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __DMA_DEFS_H__
#define __DMA_DEFS_H__

#include "types.h"

/*
// DMA 1, Stream 0
#define DMA_XY                          DMA1
#define DMA_XY_STREAM                   DMA1_Stream0
#define DMA_XY_STREAM_NR                0
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW

// DMA 1, Stream 1
#define DMA_XY                          DMA1
#define DMA_XY_STREAM                   DMA1_Stream1
#define DMA_XY_STREAM_NR                1
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW

// DMA 1, Stream 2
#define DMA_XY                          DMA1
#define DMA_XY_STREAM                   DMA1_Stream2
#define DMA_XY_STREAM_NR                2
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW

// DMA 1, Stream 3
#define DMA_XY                          DMA1
#define DMA_XY_STREAM                   DMA1_Stream3
#define DMA_XY_STREAM_NR                3
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW

// DMA 1, Stream 4
#define DMA_XY                          DMA1
#define DMA_XY_STREAM                   DMA1_Stream4
#define DMA_XY_STREAM_NR                4
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW

// DMA 1, Stream 5
#define DMA_XY_RX                       DMA1
#define DMA_XY_RX_STREAM                DMA1_Stream5
#define DMA_XY_RX_STREAM_NR             5
#define DMA_XY_RX_CHANNEL_NR            4
#define DMA_XY_RX_PRIO                  LL_DMA_PRIORITY_LOW
*/

// Debug serial TX, USART 2
#define DMA_SERIALDEBUG_TX            DMA1
#define DMA_SERIALDEBUG_TX_STREAM     DMA1_Stream6
#define DMA_SERIALDEBUG_TX_STREAM_NR  6
#define DMA_SERIALDEBUG_TX_CHANNEL_NR 4
#define DMA_SERIALDEBUG_TX_PRIO       LL_DMA_PRIORITY_LOW

/*
// DMA 1, Stream 7
#define DMA_XY                          DMA1
#define DMA_XY_STREAM                   DMA1_Stream7
#define DMA_XY_STREAM_NR                7
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW

// DMA 2, Stream 0
#define DMA_XY                          DMA2
#define DMA_XY_STREAM                   DMA2_Stream0
#define DMA_XY_STREAM_NR                0
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW

// DMA 2, Stream 1
#define DMA_XY                          DMA2
#define DMA_XY_STREAM                   DMA2_Stream1
#define DMA_XY_STREAM_NR                1
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW

// DMA 2, Stream 2
#define DMA_XY                          DMA2
#define DMA_XY_STREAM                   DMA2_Stream2
#define DMA_XY_STREAM_NR                2
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW

// DMA 2, Stream 3
#define DMA_XY                          DMA2
#define DMA_XY_STREAM                   DMA2_Stream3
#define DMA_XY_STREAM_NR                3
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW

// DMA 2, Stream 4
#define DMA_XY                          DMA2
#define DMA_XY_STREAM                   DMA2_Stream4
#define DMA_XY_STREAM_NR                4
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW

// DMA 2, Stream 5
#define DMA_XY                          DMA2
#define DMA_XY_STREAM                   DMA2_Stream5
#define DMA_XY_STREAM_NR                5
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW

// DMA 2, Stream 6
#define DMA_XY                          DMA2
#define DMA_XY_STREAM                   DMA2_Stream6
#define DMA_XY_STREAM_NR                6
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW

// DMA 2, Stream 7
#define DMA_XY                          DMA2
#define DMA_XY_STREAM                   DMA2_Stream7
#define DMA_XY_STREAM_NR                7
#define DMA_XY_CHANNEL_NR               x
#define DMA_XY_PRIO                     LL_DMA_PRIORITY_LOW
*/

#endif /* __DMA_DEFS_H__ */
