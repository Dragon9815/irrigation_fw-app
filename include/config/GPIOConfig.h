/*****************************************************************************
 * @file        GPIOConfig.h
 * @brief       GPIO configuration
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 */

#ifndef __GPIO_CONFIG_H__
#define __GPIO_CONFIG_H__

// --------------------------------------------------------------
//  UART2
// --------------------------------------------------------------
#define Config_UART_TX_Pin       GPIO_PIN_2
#define Config_UART_TX_GPIO_Port GPIOA
#define Config_UART_TX_AF        LL_GPIO_AF_7

#define Config_UART_RX_Pin       GPIO_PIN_3
#define Config_UART_RX_GPIO_Port GPIOA
#define Config_UART_RX_AF        LL_GPIO_AF_7

// --------------------------------------------------------------
//  SPI1
// --------------------------------------------------------------
#define SPI_WIZ_NSS_Pin  GPIO_PIN_4
#define SPI_WIZ_NSS_Port GPIOA
#define SPI_WIZ_NSS_AF   LL_GPIO_AF_5

#define SPI_WIZ_SCK_Pin  GPIO_PIN_5
#define SPI_WIZ_SCK_Port GPIOA
#define SPI_WIZ_SCK_AF   LL_GPIO_AF_5

#define SPI_WIZ_MISO_Pin  GPIO_PIN_6
#define SPI_WIZ_MISO_Port GPIOA
#define SPI_WIZ_MISO_AF   LL_GPIO_AF_5

#define SPI_WIZ_MOSI_Pin  GPIO_PIN_7
#define SPI_WIZ_MOSI_Port GPIOA
#define SPI_WIZ_MOSI_AF   LL_GPIO_AF_5

// --------------------------------------------------------------
//  SPI3
// --------------------------------------------------------------
#define EEPROM_SPI_SCK_Pin        GPIO_PIN_10
#define EEPROM_SPI_SCK_GPIO_Port  GPIOC
#define EEPROM_SPI_nCS_Pin        GPIO_PIN_15
#define EEPROM_SPI_nCS_GPIO_Port  GPIOA
#define EEPROM_SPI_MOSI_Pin       GPIO_PIN_5
#define EEPROM_SPI_MOSI_GPIO_Port GPIOB
#define EEPROM_SPI_MISO_Pin       GPIO_PIN_4
#define EEPROM_SPI_MISO_GPIO_Port GPIOB

#endif /* __GPIO_CONFIG_H__ */
