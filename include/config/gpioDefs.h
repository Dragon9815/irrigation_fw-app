/*****************************************************************************
 * @file        gpioDefs.h
 * @brief       GPIO definitions
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __GPIO_DEFS_H__
#define __GPIO_DEFS_H__

#include "hal/gpio/halGpio.h"

extern halGpio_config_t gpioPortA[16];
extern halGpio_config_t gpioPortB[16];
extern halGpio_config_t gpioPortC[16];
extern halGpio_config_t gpioPortD[16];

#endif /* __GPIO_DEFS_H__ */
