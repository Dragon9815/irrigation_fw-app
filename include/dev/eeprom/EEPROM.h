/*****************************************************************************
 * @file        EEPROM.h
 * @brief       EEPROM high level driver
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "Common.h"

__SG_BEGIN_DECLS

extern bool EEPROM_TryGetInitData(uint8_t initData[static 2048]); // 2048 bytes of init data -> 1024 modbus
                                                                  // registers
extern bool EEPROM_SetInitData(uint8_t initData[static 2048]);

__SG_END_DECLS

#endif /* __EEPROM_H__ */
