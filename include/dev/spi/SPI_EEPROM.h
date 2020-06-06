/*****************************************************************************
 * @file        SPI_EEPROM.h
 * @brief       SPI driver for EEPROM interface
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __SPI_EEPROM_H__
#define __SPI_EEPROM_H__

#include "dev/spi/SPI_Common.h"

__SG_BEGIN_DECLS

extern MySPI_Context_t SPI_EEPROM_Context;

extern void SPI_EEPROM_Initalize(void);
extern void SPI_EEPROM_ExecuteThread(void);

__SG_END_DECLS

#endif /* __SPI_EEPROM_H__ */
