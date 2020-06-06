/*****************************************************************************
 * @file        crc16.h
 * @brief       CRC16 calculator
 * @author      Attila.Arokszallasi@wallner-automation.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __UTILS_CRC16_H__
#define __UTILS_CRC16_H__

#include "Common.h"

extern uint16_t CRC16_Calculate(const uint8_t Data[], int Length);
extern uint16_t CRC16_CalculatePart(const uint8_t Data[], int Length, uint16_t PartialCRC);

#endif /* __UTILS_CRC16_H__ */
