/*****************************************************************************
 * @file        SPI_WIZ.h
 * @brief       SPI driver for WIZ ethernet driver
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __SPI_WIZ_H__
#define __SPI_WIZ_H__

#include "dev/spi/SPI_Common.h"

__SG_BEGIN_DECLS

extern MySPI_Context_t SPI_WIZ_Context;

extern void SPI_WIZ_Initalize(void);
extern void SPI_WIZ_ExecuteThread(void);

__SG_END_DECLS

#endif /* __SPI_WIZ_H__ */
