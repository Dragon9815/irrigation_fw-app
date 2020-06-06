/*****************************************************************************
 * @file        libgpio.h
 * 
 * @brief       GPIO HAL layer       
 * @ingroup     HAL
 * 
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

#include "types.h"
#include <stm32f4xx.h>

#define HAL_GPIO_PIN_0   0x0001
#define HAL_GPIO_PIN_1   0x0002
#define HAL_GPIO_PIN_2   0x0004
#define HAL_GPIO_PIN_3   0x0008
#define HAL_GPIO_PIN_4   0x0010
#define HAL_GPIO_PIN_5   0x0020
#define HAL_GPIO_PIN_6   0x0040
#define HAL_GPIO_PIN_7   0x0080
#define HAL_GPIO_PIN_8   0x0100
#define HAL_GPIO_PIN_9   0x0200
#define HAL_GPIO_PIN_10  0x0400
#define HAL_GPIO_PIN_11  0x0800
#define HAL_GPIO_PIN_12  0x1000
#define HAL_GPIO_PIN_13  0x2000
#define HAL_GPIO_PIN_14  0x4000
#define HAL_GPIO_PIN_15  0x8000

#define HAL_GPIO_AF_NONE 0

// AF 1
#define HAL_GPIO_AF1_TIM1 1
#define HAL_GPIO_AF1_TIM2 1

// AF 2
#define HAL_GPIO_AF2_TIM3 2
#define HAL_GPIO_AF2_TIM4 2
#define HAL_GPIO_AF2_TIM5 2

// AF 3
#define HAL_GPIO_AF3_TIM8  3
#define HAL_GPIO_AF3_TIM9  3
#define HAL_GPIO_AF3_TIM10 3
#define HAL_GPIO_AF3_TIM11 3
#define HAL_GPIO_AF3_CEC   3

// AF 4
#define HAL_GPIO_AF4_I2C1 4
#define HAL_GPIO_AF4_I2C2 4
#define HAL_GPIO_AF4_I2C3 4
#define HAL_GPIO_AF4_I2C4 4
#define HAL_GPIO_AF4_CEC  4

// AF 5
#define HAL_GPIO_AF5_SPI1 5
#define HAL_GPIO_AF5_SPI2 5
#define HAL_GPIO_AF5_SPI3 5
#define HAL_GPIO_AF5_SPI4 5

// AF 6
#define HAL_GPIO_AF6_SPI2 6
#define HAL_GPIO_AF6_SPI3 6
#define HAL_GPIO_AF6_SPI4 6
#define HAL_GPIO_AF6_SAI1 6

// AF 7
#define HAL_GPIO_AF7_SPI2    7
#define HAL_GPIO_AF7_SPI3    7
#define HAL_GPIO_AF7_USART1  7
#define HAL_GPIO_AF7_USART2  7
#define HAL_GPIO_AF7_USART3  7
#define HAL_GPIO_AF7_UART5   7
#define HAL_GPIO_AF7_SPDIFRX 7

// AF 8
#define HAL_GPIO_AF8_SAI2    8
#define HAL_GPIO_AF8_USART6  8
#define HAL_GPIO_AF8_UART4   8
#define HAL_GPIO_AF8_UART5   8
#define HAL_GPIO_AF8_SPDIFRX 8

// AF 9
#define HAL_GPIO_AF9_CAN1    9
#define HAL_GPIO_AF9_CAN2    9
#define HAL_GPIO_AF9_TIM12   9
#define HAL_GPIO_AF9_TIM13   9
#define HAL_GPIO_AF9_TIM14   9
#define HAL_GPIO_AF9_QUADSPI 9

// AF 10
#define HAL_GPIO_AF10_SAI2    10
#define HAL_GPIO_AF10_QUADSPI 10
#define HAL_GPIO_AF10_OTG2_HS 10
#define HAL_GPIO_AF10_OTG1_FS 10

// AF 11
#define HAL_GPIO_AF11_OTG1_FS 11

// AF 12
#define HAL_GPIO_AF12_FMC     12
#define HAL_GPIO_AF12_SDIO    12
#define HAL_GPIO_AF12_OTG2_FS 12

// AF 13
#define HAL_GPIO_AF13_DCMI    13

// AF 14

// AF 15
#define HAL_GPIO_AF15_EVENTOUT 15

typedef enum 
{
    HAL_GPIO_MODE_INPUT     = 0x00,
    HAL_GPIO_MODE_OUTPUT    = 0x01,
    HAL_GPIO_MODE_ALTERNATE = 0x02,
    HAL_GPIO_MODE_ANALOG    = 0x03,

    // special modes
    HAL_GPIO_MODE_UNUSED    = 0xE0,
    HAL_GPIO_MODE_UNCHANGED = 0xF0,
} halGpio_mode_t;

typedef enum
{
    HAL_GPIO_OUTPUT_TYPE_PUSHPULL = 0,
    HAL_GPIO_OUTPUT_TYPE_OPENDRAIN = 1
} halGpio_outputType_t;

typedef enum
{
    HAL_GPIO_SPEED_LOW    = 0x00,
    HAL_GPIO_SPEED_MEDIUM = 0x01,
    HAL_GPIO_SPEED_FAST   = 0x02,
    HAL_GPIO_SPEED_HIGH   = 0x03,
} halGpio_speed_t;

typedef enum
{
    HAL_GPIO_PULL_NO   = 0x00,
    HAL_GPIO_PULL_UP   = 0x01,
    HAL_GPIO_PULL_DOWN = 0x02,
} halGpio_pull_t;

typedef enum
{
    HAL_GPIO_INIT_SET,
    HAL_GPIO_INIT_RESET
} halGpio_init_t;

typedef struct
{
    halGpio_mode_t       mode;
    halGpio_outputType_t outType;
    halGpio_pull_t       pull;
    halGpio_speed_t      speed;
    u8                  alternate;
    halGpio_init_t       init;
} halGpio_config_t;

void halGpio_EnableClock(GPIO_TypeDef * port, bool enable);

void halGpio_InitPort(GPIO_TypeDef * port, halGpio_config_t * config);
void halGpio_InitPortPin(GPIO_TypeDef * port, u32 pinMask, halGpio_config_t * config);

void halGpio_SetAlternateFunction(GPIO_TypeDef * port, u8 pinIndex, u8 alternate);

void halGpio_SetPin(GPIO_TypeDef * port, u16 pinMask);
void halGpio_ResetPin(GPIO_TypeDef * port, u16 pinMask);
void halGpio_TogglePin(GPIO_TypeDef * port, u16 pinMask);
void halGpio_SetPort(GPIO_TypeDef * port, u16 portData);

bool halGpio_GetPin(GPIO_TypeDef * port, u16 pinMask);
u16  halGpio_GetPort(GPIO_TypeDef * port);

#endif /* __HAL_GPIO_H__ */

