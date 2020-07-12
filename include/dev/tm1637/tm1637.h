#ifndef __DEV_TM1637_H__
#define __DEV_TM1637_H__

#include "Common.h"

static const uint8_t digitToSegment[] = {
    //XGFEDCBA
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // b
    0b00111001, // C
    0b01011110, // d
    0b01111001, // E
    0b01110001  // F
};

extern void tm1637_initialize(void);
extern void tm1637_sendDisplayData(const uint8_t displayBuffer[static 4], bool dotEnabled);

#endif /* __DEV_TM1637_H__ */