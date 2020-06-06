/*****************************************************************************
 * @file        Utils.c
 * @brief       Various utility functions
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "Common.h"

void FillZero(void * buffer, size_t capacity)
{
    uint8_t * _buffer = (uint8_t *)buffer;
    for(size_t i = 0; i < capacity; i++) {
        _buffer[i] = 0;
    }
}
