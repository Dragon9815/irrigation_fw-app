///*****************************************************************************
// * @file        SPI_EEPROM.c
// * @brief       SPI driver for EEPROM interface
// * @author      gruenzinger.stefan@gmail.com
// * @copyright   Copyright (c) 2020 Stefan Grünzinger
// *****************************************************************************
// */
//
//#include "dev/spi/SPI_EEPROM.h"
//
//#include "config/GPIOConfig.h"
//
// MySPI_Context_t   SPI_EEPROM_Context;
// SPI_HandleTypeDef hspi3;
//
// void SPI_EEPROM_Initalize( void )
//{
//    hspi3.Instance               = SPI3;
//    hspi3.Init.Mode              = SPI_MODE_MASTER;
//    hspi3.Init.Direction         = SPI_DIRECTION_2LINES;
//    hspi3.Init.DataSize          = SPI_DATASIZE_8BIT;
//    hspi3.Init.CLKPolarity       = SPI_POLARITY_LOW;
//    hspi3.Init.CLKPhase          = SPI_PHASE_1EDGE;
//    hspi3.Init.NSS               = SPI_NSS_SOFT;
//    hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
//    hspi3.Init.FirstBit          = SPI_FIRSTBIT_MSB;
//    hspi3.Init.TIMode            = SPI_TIMODE_DISABLED;
//    hspi3.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
//    hspi3.Init.CRCPolynomial     = 10;
//    if( HAL_SPI_Init( &hspi3 ) != HAL_OK )
//    {
//        Error_Handler( );
//    }
//
//    MySPI_InitContext( &SPI_EEPROM_Context, &hspi3, GPIOA, GPIO_PIN_15 );
//}
//
// void SPI3_MspInit( void )
//{
//    __HAL_RCC_SPI3_CLK_ENABLE( );
//
//    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
//
//    GPIO_InitStruct.Pin       = EEPROM_SPI_MISO_Pin | EEPROM_SPI_MOSI_Pin;
//    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull      = GPIO_NOPULL;
//    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
//    HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
//
//    GPIO_InitStruct.Pin       = EEPROM_SPI_SCK_Pin;
//    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull      = GPIO_NOPULL;
//    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
//    HAL_GPIO_Init( EEPROM_SPI_SCK_GPIO_Port, &GPIO_InitStruct );
//
//    GPIO_InitStruct.Pin   = EEPROM_SPI_nCS_Pin;
//    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull  = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    HAL_GPIO_Init( EEPROM_SPI_nCS_GPIO_Port, &GPIO_InitStruct );
//
//    HAL_NVIC_SetPriority( SPI3_IRQn, 0, 0 );
//    HAL_NVIC_EnableIRQ( SPI3_IRQn );
//}
//
// void SPI3_MspDeInit( void )
//{
//    HAL_NVIC_DisableIRQ( SPI3_IRQn );
//
//    __HAL_RCC_SPI3_CLK_DISABLE( );
//
//    HAL_GPIO_DeInit( GPIOB, EEPROM_SPI_MISO_Pin | EEPROM_SPI_MOSI_Pin );
//    HAL_GPIO_DeInit( EEPROM_SPI_SCK_GPIO_Port, EEPROM_SPI_SCK_Pin );
//}
//
// void SPI3_TxCpltCallback( void )
//{
//    SPI_EEPROM_Context.txCompleted = true;
//}
//
// void SPI3_RxCpltCallback( void )
//{
//    SPI_EEPROM_Context.rxCompleted = true;
//}
//
// void SPI3_IRQHandler( void )
//{
//    HAL_SPI_IRQHandler( &hspi3 );
//}
