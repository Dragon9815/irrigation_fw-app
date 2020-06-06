/*****************************************************************************
 * @file        SPI_Common.h
 * @brief       Stuff common to all SPI interfaces
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __SPI_COMMON_H__
#define __SPI_COMMON_H__

#include "Common.h"
#include "mcu.h"

__SG_BEGIN_DECLS

// typedef void ( *MySPI_RxCallback_t )( uint8_t rxByte );
//
// typedef struct
//{
//    SPI_HandleTypeDef * hspi;
//
//    GPIO_TypeDef * nssPort;
//    uint32_t       nssPin;
//
//    bool     rxRequest;
//    bool     rxCompleted;
//    bool     rxErrorFlag;
//    uint32_t rxErrorCount;
//
//    bool     txRequest;
//    bool     txCompleted;
//    bool     txErrorFlag;
//    uint32_t txErrorCount;
//} MySPI_Context_t;
//
// typedef enum
//{
//    MySPI_Result_Ok,      /* Data was successfully received / transmitted */
//    MySPI_Result_Timeout, /* Data didn't arrive / wasn't sent in specified time,
//                             it will still be received/snt in the
//                                                     background and provided by
//                             further calls */
//    MySPI_Result_Error,   /* Error occurred while receiving/transmitting data */
//} MySPI_Result_t;
//
// extern void MySPI_InitContext( MySPI_Context_t * ctx, SPI_HandleTypeDef * hspi,
//    GPIO_TypeDef * nssPort, uint32_t nssPin );
//
// extern void MySPI_SetNSSState( MySPI_Context_t * ctx, bool state );
// extern void MySPI_NSSSet( MySPI_Context_t * ctx );
// extern void MySPI_NSSReset( MySPI_Context_t * ctx );
//
// extern MySPI_Result_t MySPI_TransmitByte(
//    MySPI_Context_t * ctx, uint8_t byte, uint32_t timeout );
// extern MySPI_Result_t MySPI_TransmitByteArray( MySPI_Context_t * ctx,
//    const uint8_t * bytes, size_t byteCount, uint32_t timeout );
//
// extern MySPI_Result_t MySPI_ReceiveByte(
//    MySPI_Context_t * ctx, _OUT_ uint8_t * byte, uint32_t timeout );
// extern MySPI_Result_t MySPI_ReceiveBytes( MySPI_Context_t * ctx,
//    _OUT_ uint8_t * byte, size_t byteCount, uint32_t timeout );

typedef struct
{
    SPI_TypeDef * periph;

    GPIO_TypeDef * nss_port;
    u32            nss_pin;
} MySPI_Context_t;

extern void MySPI_InitContext(MySPI_Context_t * ctx, SPI_TypeDef * periph);
extern void MySPI_InitNSS(MySPI_Context_t * ctx, GPIO_TypeDef * port, u32 pin);

extern void MySPI_SetNSS(MySPI_Context_t * ctx);
extern void MySPI_ResetNSS(MySPI_Context_t * ctx);

extern u8   MySPI_Transfer(MySPI_Context_t * ctx, u8 byte);
extern void MySPI_TransferArray(MySPI_Context_t * ctx, u8 * buffer, size_t count);
extern void MySPI_ReceiveArray(MySPI_Context_t * ctx, u8 * buffer, size_t count);

__SG_END_DECLS

#endif /* __SPI_COMMON_H__ */
