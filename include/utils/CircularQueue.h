/*****************************************************************************
 * @file        CircularQueue.h
 * @brief       Operate on circular queues
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __CIRCULAR_QUEUE_H__
#define __CIRCULAR_QUEUE_H__

#include "Common.h"

typedef struct
{
    uint8_t * Data;

    uint32_t Length;
    uint32_t DataCount;

    uint32_t WriteIndex;
    uint32_t ReadIndex;

    struct
    {
        uint32_t OverflowCounter;
        uint32_t UnderflowCounter;
    } ErrorInfo;
} CircularQueue_t;

__SG_BEGIN_DECLS

extern void CircQueue_CreateFromStack(CircularQueue_t * buf, uint32_t bufLength, uint8_t * data);
extern void CircQueue_CreateFromHeap(CircularQueue_t * buf, uint32_t bufLength);

extern void    CircQueue_EnqueueByte(CircularQueue_t * buf, uint8_t byte);
extern uint8_t CircQueue_DequeueByte(CircularQueue_t * buf);

extern uint32_t CircQueue_Count(CircularQueue_t * buf);

extern void CircQueue_Clear(CircularQueue_t * buf);

__SG_END_DECLS

#endif /* __CIRCULAR_QUEUE_H__ */
