/*****************************************************************************
 * @file        utils/gpio.h
 * @brief       GPIO utility functions
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __UTILS_GPIO_H__
#define __UTILS_GPIO_H__

#define GPIO_Init_Input_Floating(port, pins)                \
    {                                                       \
        GPIO_InitTypeDef GPIO_InitStruct = { 0 };           \
        GPIO_InitStruct.Pin              = (pins);          \
        GPIO_InitStruct.Mode             = GPIO_MODE_INPUT; \
        GPIO_InitStruct.Pull             = GPIO_NOPULL;     \
        HAL_GPIO_Init(port, &GPIO_InitStruct);              \
    }

#define GPIO_Init_Input_PullUp(port, pins)                  \
    {                                                       \
        GPIO_InitTypeDef GPIO_InitStruct = { 0 };           \
        GPIO_InitStruct.Pin              = (pins);          \
        GPIO_InitStruct.Mode             = GPIO_MODE_INPUT; \
        GPIO_InitStruct.Pull             = GPIO_PULLUP;     \
        HAL_GPIO_Init(port, &GPIO_InitStruct);              \
    }

#define GPIO_Init_Input_PullDown(port, pins)                \
    {                                                       \
        GPIO_InitTypeDef GPIO_InitStruct = { 0 };           \
        GPIO_InitStruct.Pin              = (pins);          \
        GPIO_InitStruct.Mode             = GPIO_MODE_INPUT; \
        GPIO_InitStruct.Pull             = GPIO_PULLDOWN;   \
        HAL_GPIO_Init(port, &GPIO_InitStruct);              \
    }

#define GPIO_Init_Output_PushPull(port, pins, speed)            \
    {                                                           \
        GPIO_InitTypeDef GPIO_InitStruct = { 0 };               \
        GPIO_InitStruct.Pin              = (pins);              \
        GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_PP; \
        GPIO_InitStruct.Speed            = speed;               \
        HAL_GPIO_Init(port, &GPIO_InitStruct);                  \
    }

#define GPIO_Init_Output_OpenDrain(port, pins, speed)           \
    {                                                           \
        GPIO_InitTypeDef GPIO_InitStruct = { 0 };               \
        GPIO_InitStruct.Pin              = (pins);              \
        GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_OD; \
        GPIO_InitStruct.Speed            = speed;               \
        HAL_GPIO_Init(port, &GPIO_InitStruct);                  \
    }

#endif /* __UTILS_GPIO_H__ */
