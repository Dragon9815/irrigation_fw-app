/*****************************************************************************
 * @file        System.c
 * @brief       Low Level System code
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "system/System.h"

#include "Common.h"
#include "mcu.h"

void SystemClock_Config(void);

extern uint32_t g_pfnVectors[];

void System_Initialize(void)
{
    SCB->VTOR = (uint32_t)g_pfnVectors;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    HAL_Init();
    SystemClock_Config();

    // initalize sytick
    uint32_t TicksNumb = SystemCoreClock / 1000U; // 1 kHz
    SysTick_Config(TicksNumb);

    __enable_irq();
}

void SystemClock_Config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

    if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5) {
        return;
    }

    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    LL_PWR_EnableOverDriveMode();

    LL_RCC_HSE_Enable();
    while(LL_RCC_HSE_IsReady() != 1)
        ;

    LL_PWR_EnableBkUpAccess();
    // LL_RCC_ForceBackupDomainReset();
    // LL_RCC_ReleaseBackupDomainReset();

    LL_RCC_LSE_Enable();
    while(LL_RCC_LSE_IsReady() != 1)
        ;

    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
    LL_RCC_EnableRTC();

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 180, LL_RCC_PLLI2SR_DIV_2);
    LL_RCC_PLL_Enable();
    while(LL_RCC_PLL_IsReady())
        ;

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
        ;

    SystemCoreClockUpdate();
    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);

    //	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    //	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
    //
    //	/**
    //	 * Configure the main internal regulator output voltage
    //	 */
    //	__HAL_RCC_PWR_CLK_ENABLE();
    //	__HAL_PWR_VOLTAGESCALING_CONFIG( PWR_REGULATOR_VOLTAGE_SCALE1 );
    //
    //	/**
    //	 * Initializes the CPU, AHB and APB busses clocks
    //	 */
    //	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    //	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    //	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    //	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    //	RCC_OscInitStruct.PLL.PLLM = 6;
    //	RCC_OscInitStruct.PLL.PLLN = 180;
    //	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    //	RCC_OscInitStruct.PLL.PLLQ = 2;
    //	RCC_OscInitStruct.PLL.PLLR = 2;
    //	if ( HAL_RCC_OscConfig( &RCC_OscInitStruct ) != HAL_OK )
    //	{
    //		Error_Handler( );
    //	}
    //
    //	/**
    //	 * Activate the Over-Drive mode
    //	 */
    //	if ( HAL_PWREx_EnableOverDrive( ) != HAL_OK )
    //	{
    //		Error_Handler( );
    //	}
    //
    //	/**
    //	 * Initializes the CPU, AHB and APB busses clocks
    //	 */
    //	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
    //| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    //	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLRCLK;
    //	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    //	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    //	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    //
    //	if ( HAL_RCC_ClockConfig( &RCC_ClkInitStruct, FLASH_LATENCY_5 ) !=
    // HAL_OK )
    //	{
    //		Error_Handler( );
    //	}
}

void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}
