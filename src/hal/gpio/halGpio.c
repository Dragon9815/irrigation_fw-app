/*****************************************************************************
 * @file        libgpio.c
 * 
 * @brief       GPIO HAL layer       
 * @ingroup     HAL
 * 
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "hal/gpio/halGpio.h"

void halGpio_EnableClock(GPIO_TypeDef * port, bool enable)
{
#ifdef GPIOA
    if(port == GPIOA) {
        if(enable) {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        }
        else {
            RCC->AHB1ENR &= RCC_AHB1ENR_GPIOAEN;
        }
    }
#endif /* GPIOA */

#ifdef GPIOB
    if(port == GPIOB) {
        if(enable) {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        }
        else {
            RCC->AHB1ENR &= RCC_AHB1ENR_GPIOBEN;
        }
    }
#endif /* GPIOB */

#ifdef GPIOC
    if(port == GPIOC) {
        if(enable) {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        }
        else {
            RCC->AHB1ENR &= RCC_AHB1ENR_GPIOCEN;
        }
    }
#endif /* GPIOC */

#ifdef GPIOD
    if(port == GPIOD) {
        if(enable) {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
        }
        else {
            RCC->AHB1ENR &= RCC_AHB1ENR_GPIODEN;
        }
    }
#endif /* GPIOD */

#ifdef GPIOE
    if(port == GPIOE) {
        if(enable) {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
        }
        else {
            RCC->AHB1ENR &= RCC_AHB1ENR_GPIOEEN;
        }
    }
#endif /* GPIOE */

#ifdef GPIOF
    if(port == GPIOF) {
        if(enable) {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
        }
        else {
            RCC->AHB1ENR &= RCC_AHB1ENR_GPIOFEN;
        }
    }
#endif /* GPIOF */

#ifdef GPIOG
    if(port == GPIOG) {
        if(enable) {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
        }
        else {
            RCC->AHB1ENR &= RCC_AHB1ENR_GPIOGEN;
        }
    }
#endif /* GPIOG */

#ifdef GPIOH
    if(port == GPIOH) {
        if(enable) {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
        }
        else {
            RCC->AHB1ENR &= RCC_AHB1ENR_GPIOHEN;
        }
    }
#endif /* GPIOH */

#ifdef GPIOI
    if(port == GPIOI) {
        if(enable) {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
        }
        else {
            RCC->AHB1ENR &= RCC_AHB1ENR_GPIOIEN;
        }
    }
#endif /* GPIOI */

#ifdef GPIOJ
    if(port == GPIOJ) {
        if(enable) {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
        }
        else {
            RCC->AHB1ENR &= RCC_AHB1ENR_GPIOJEN;
        }
    }
#endif /* GPIOJ */

#ifdef GPIOK
    if(port == GPIOK) {
        if(enable) {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOKEN;
        }
        else {
            RCC->AHB1ENR &= RCC_AHB1ENR_GPIOKEN;
        }
    }
#endif /* GPIOK */
}

static void halGpio_InitPin(GPIO_TypeDef * port, u8 pinIndex, halGpio_config_t * config)
{
    if((port == NULL) || (pinIndex > 15)) {
        return;
    }

    if(config->mode == HAL_GPIO_MODE_UNCHANGED) {
        return;
    }

    halGpio_EnableClock(port, true);

    // Setup all unused pins as inputs with internal pull-down
    if(config->mode == HAL_GPIO_MODE_UNUSED) {
        config->mode = HAL_GPIO_MODE_INPUT;
        config->pull = HAL_GPIO_PULL_DOWN;
    }

    if(config->mode == HAL_GPIO_MODE_OUTPUT) {
        // Set initial output value
        if(config->init == HAL_GPIO_INIT_SET) {
            halGpio_SetPin(port, (1 << pinIndex));
        }
        else {
            halGpio_ResetPin(port, (1 << pinIndex));
        }
    }

    // Set mode register
    port->MODER &= ~(GPIO_MODER_MODE0 << (pinIndex * 2));
    port->MODER |= (((u32)(config->mode) & 0x03) << (pinIndex * 2));

    // Set pull-up/pull-down
    port->PUPDR &= ~(GPIO_PUPDR_PUPD0 << (pinIndex * 2));
    port->PUPDR |= (((u32)(config->pull) & 0x03) << (pinIndex * 2));

    if((config->mode == HAL_GPIO_MODE_OUTPUT) || (config->mode == HAL_GPIO_MODE_ALTERNATE)) {
        // Set output type
        port->OTYPER &= ~(GPIO_OTYPER_OT0 << pinIndex);
        port->OTYPER |= (((u32)(config->outType) & 0x01) << pinIndex);

        // Set speed
        port->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED0 << (pinIndex * 2));
        port->OSPEEDR |= (((u32)(config->speed) & 0x03) << (pinIndex * 2));
    }

    if(config->mode == HAL_GPIO_MODE_ALTERNATE) {
        // Set alternate function
        halGpio_SetAlternateFunction(port, pinIndex, config->alternate);
    }
}


void halGpio_InitPort(GPIO_TypeDef * port, halGpio_config_t * config)
{
    int pinIndex;

    for(pinIndex = 0; pinIndex < 16; pinIndex++) {
        halGpio_InitPin(port, pinIndex, config);
        config++;
    }
}

void halGpio_InitPortPin(GPIO_TypeDef * port, u32 pinMask, halGpio_config_t * config)
{
    int pinIndex;

    for(pinIndex = 0; pinIndex < 16; pinIndex++) {
        if((pinMask & (1 << pinIndex)) != 0) {
            halGpio_InitPin(port, pinIndex, config);
        }
    }
}

void halGpio_SetAlternateFunction(GPIO_TypeDef * port, u8 pinIndex, u8 alternate)
{
    u8 afrIndex;
    u8 afrOffset;

    // 4 bits AF per pin
    // calculate which register to use AFH/AFL
    afrIndex = pinIndex >> 3;
    // calculate offset into register
    afrOffset = (pinIndex & 0x07) * 4;

    port->AFR[afrIndex] &= ~(GPIO_AFRL_AFSEL0_Msk << afrOffset);
    port->AFR[afrIndex] |= ((u32)alternate << afrOffset);
}

void halGpio_SetMode(GPIO_TypeDef * port, u8 pinIndex, halGpio_mode_t mode)
{
    if(mode != HAL_GPIO_MODE_UNCHANGED && mode != HAL_GPIO_MODE_UNUSED) {
        port->MODER &= ~(GPIO_MODER_MODE0 << (pinIndex * 2));
        port->MODER |= (((u32)(mode)&0x03) << (pinIndex * 2));
    }
}

void halGpio_SetPin(GPIO_TypeDef * port, u16 pinMask)
{
    port->BSRR = (u32)pinMask;
}

void halGpio_ResetPin(GPIO_TypeDef * port, u16 pinMask)
{
    port->BSRR = ((u32)(pinMask)) << 16;
}

void halGpio_TogglePin(GPIO_TypeDef * port, u16 pinMask)
{
    port->ODR ^= pinMask;
}

void halGpio_SetPort(GPIO_TypeDef * port, u16 portData)
{
    port->ODR = portData;
}

bool halGpio_GetPin(GPIO_TypeDef * port, u16 pinMask)
{
    return ((port->IDR & pinMask) == pinMask);
}

u16 halGpio_GetPort(GPIO_TypeDef * port)
{
    return port->IDR;
}
