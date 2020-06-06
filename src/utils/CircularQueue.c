/*****************************************************************************
 * @file        CircularQueue.c
 * @brief       Operate on circular queues
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "utils/CircularQueue.h"

#include "utils/memory.h"

#include <stdlib.h>

void CircQueue_CreateFromStack(CircularQueue_t * buf, uint32_t bufLength, uint8_t * data)
{
    FillZero(buf, sizeof(CircularQueue_t));
    buf->Data   = data;
    buf->Length = bufLength;
}

void CircQueue_CreateFromHeap(CircularQueue_t * buf, uint32_t bufLength)
{
    FillZero(buf, sizeof(CircularQueue_t));
    buf->Data   = (uint8_t *)malloc(bufLength);
    buf->Length = bufLength;
}

void CircQueue_EnqueueByte(CircularQueue_t * buf, uint8_t byte)
{
    if(buf->DataCount >= buf->Length) {
        buf->ErrorInfo.OverflowCounter++;
        return;
    }

    buf->Data[buf->WriteIndex] = byte;
    buf->WriteIndex++;
    if(buf->WriteIndex >= buf->Length)
        buf->WriteIndex = 0;
    buf->DataCount++;
}

uint8_t CircQueue_DequeueByte(CircularQueue_t * buf)
{
    if(buf->DataCount <= 0) {
        buf->ErrorInfo.UnderflowCounter++;
        return 0;
    }

    uint8_t byte = buf->Data[buf->ReadIndex];
    buf->ReadIndex++;
    if(buf->ReadIndex >= buf->Length)
        buf->ReadIndex = 0;
    buf->DataCount--;
    return byte;
}

uint32_t CircQueue_Count(CircularQueue_t * buf)
{
    return buf->DataCount;
}

void CircQueue_Clear(CircularQueue_t * buf)
{
    buf->ReadIndex  = 0;
    buf->WriteIndex = 0;
    buf->DataCount  = 0;
}
