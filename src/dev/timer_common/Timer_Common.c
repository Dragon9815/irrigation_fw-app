/*****************************************************************************
 * @file        Timer_Common.c
 * @brief       Code common to all hardware timers
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "Common.h"
#include "mcu.h"

// Add more functions here
#define TIM_DeclFunctions(tim)                    \
    __weak void tim##_MspInit(void)               \
    {}                                            \
    __weak void tim##_MspDeInit(void)             \
    {}                                            \
    __weak void tim##_MspPostInit(void)           \
    {}                                            \
    __weak void tim##_PeriodElapsedCallback(void) \
    {}

#define TIM_ConditionFirst(tim, func) \
    if(htim->Instance == tim) {       \
        tim##_##func();               \
    }

#define TIM_ConditionOther(tim, func) \
    else if(htim->Instance == tim)    \
    {                                 \
        tim##_##func();               \
    }

// Add additional timers here
#define TIM_Conditions(func) \
    TIM_ConditionFirst(TIM5, func) TIM_ConditionFirst(TIM6, func) TIM_ConditionFirst(TIM7, func)


#define TIM_ProxyFunc(func)                       \
    void HAL_TIM_##func(TIM_HandleTypeDef * htim) \
    {                                             \
        TIM_Conditions(func)                      \
    }

#define TIM_ProxyFuncExtra(func, extra)             \
    void HAL_TIM_##func(UART_HandleTypeDef * huart) \
    {                                               \
        TIM_Conditions(func) extra;                 \
    }

#define TIM_ProxyFunc2(func, proxy)               \
    void HAL_TIM_##func(TIM_HandleTypeDef * htim) \
    {                                             \
        TIM_Conditions(proxy)                     \
    }


// Add additional timers here
TIM_DeclFunctions(TIM5) TIM_DeclFunctions(TIM6) TIM_DeclFunctions(TIM7)

  // Add more functions here
  TIM_ProxyFunc2(Base_MspInit, MspInit) TIM_ProxyFunc2(OC_MspInit, MspInit) TIM_ProxyFunc2(IC_MspInit, MspInit)
    TIM_ProxyFunc2(PWM_MspInit, MspInit) TIM_ProxyFunc2(Encoder_MspInit, MspInit)

      TIM_ProxyFunc2(Base_MspDeInit, MspDeInit) TIM_ProxyFunc2(OC_MspDeInit, MspDeInit)
        TIM_ProxyFunc2(IC_MspDeInit, MspDeInit) TIM_ProxyFunc2(PWM_MspDeInit, MspDeInit)
          TIM_ProxyFunc2(Encoder_MspDeInit, MspDeInit)

            TIM_ProxyFunc(MspPostInit);
TIM_ProxyFunc(PeriodElapsedCallback);
