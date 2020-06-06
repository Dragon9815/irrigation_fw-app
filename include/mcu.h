/*****************************************************************************
 * @file        MCU.h
 * @brief       Includes all needed libraries for the MCU
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */
#ifndef __MCU_H__
#define __MCU_H__

#include <stm32f4xx.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <stm32f4xx_hal.h>
#include <stm32f4xx_ll_adc.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_cortex.h>
#include <stm32f4xx_ll_dma.h>
#include <stm32f4xx_ll_exti.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_pwr.h>
#include <stm32f4xx_ll_rcc.h>
#include <stm32f4xx_ll_spi.h>
#include <stm32f4xx_ll_system.h>
#include <stm32f4xx_ll_tim.h>
#include <stm32f4xx_ll_usart.h>
#include <stm32f4xx_ll_utils.h>

#pragma GCC diagnostic pop

#endif /* __MCU_H__ */
