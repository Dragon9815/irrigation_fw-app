/*****************************************************************************
 * @file        SPI_WIZ.h
 * @brief       SPI driver for WIZ ethernet driver
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "dev/spi/SPI_WIZ.h"

#include "config/GPIOConfig.h"
#include "mcu.h"

#define SPI_WIZ      SPI1
#define SPI_WIZ_IRQn SPI1_IRQn

MySPI_Context_t SPI_WIZ_Context;

void SPI_WIZ_Initalize(void)
{
    LL_SPI_InitTypeDef  SPI_InitStruct  = { 0 };
    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    // reset peripheral
    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_SPI1);
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_SPI1);

    // enable peripheral clock
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

    // setup gpios
    GPIO_InitStruct.Pin       = SPI_WIZ_NSS_Pin;
    GPIO_InitStruct.Mode      = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed     = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull      = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = SPI_WIZ_NSS_AF;
    LL_GPIO_Init(SPI_WIZ_NSS_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = SPI_WIZ_SCK_Pin;
    GPIO_InitStruct.Mode      = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed     = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull      = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = SPI_WIZ_SCK_AF;
    LL_GPIO_Init(SPI_WIZ_SCK_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = SPI_WIZ_MOSI_Pin;
    GPIO_InitStruct.Mode      = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed     = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull      = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = SPI_WIZ_MOSI_AF;
    LL_GPIO_Init(SPI_WIZ_MOSI_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = SPI_WIZ_MISO_Pin;
    GPIO_InitStruct.Mode      = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed     = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull      = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = SPI_WIZ_MISO_AF;
    LL_GPIO_Init(SPI_WIZ_MISO_Port, &GPIO_InitStruct);

    // setup interrupt vector
    //    NVIC_SetPriority( SPI_WIZ_IRQn,
    //                      NVIC_EncodePriority( NVIC_GetPriorityGrouping( ), 0, 0 ) );
    //    NVIC_EnableIRQ( SPI_WIZ_IRQn );

    SPI_InitStruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV64;
    SPI_InitStruct.BitOrder          = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly           = 0;
    SPI_InitStruct.ClockPhase        = LL_SPI_PHASE_1EDGE;
    SPI_InitStruct.ClockPolarity     = LL_SPI_POLARITY_LOW;
    SPI_InitStruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.Mode              = LL_SPI_MODE_MASTER;
    SPI_InitStruct.NSS               = LL_SPI_NSS_SOFT;
    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    LL_SPI_Init(SPI_WIZ, &SPI_InitStruct);

    //    LL_SPI_EnableIT_RXNE( SPI_WIZ );
    //    LL_SPI_EnableIT_TXE( SPI_WIZ );
    //    LL_SPI_EnableIT_ERR( SPI_WIZ );

    MySPI_InitContext(&SPI_WIZ_Context, SPI_WIZ);
    MySPI_InitNSS(&SPI_WIZ_Context, SPI_WIZ_NSS_Port, SPI_WIZ_NSS_Pin);
    MySPI_ResetNSS(&SPI_WIZ_Context);
}

void SPI_WIZ_ExecuteThread(void)
{}

// void SPI1_IRQHandler( void )
//{
//}
