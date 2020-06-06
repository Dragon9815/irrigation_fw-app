/*****************************************************************************
 * @file        SPI_Common.c
 * @brief       Stuff common to all SPI interfaces
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "dev/spi/SPI_Common.h"

#include "mcu.h"
#include "utils/memory.h"

#define VOID
#define RETURN_ON_NULL(x, r) \
    if((x) == NULL) {        \
        return r;            \
    }

void MySPI_InitContext(MySPI_Context_t * ctx, SPI_TypeDef * periph)
{
    RETURN_ON_NULL(ctx, VOID);
    FillZero(ctx, sizeof(MySPI_Context_t));
    ctx->periph = periph;
}

void MySPI_InitNSS(MySPI_Context_t * ctx, GPIO_TypeDef * port, u32 pin)
{
    RETURN_ON_NULL(ctx, VOID);
    ctx->nss_port = port;
    ctx->nss_pin  = pin;
}

void MySPI_SetNSS(MySPI_Context_t * ctx)
{
    RETURN_ON_NULL(ctx, VOID);
    RETURN_ON_NULL(ctx->nss_port, VOID);

    LL_GPIO_ResetOutputPin(ctx->nss_port, ctx->nss_pin);
}

void MySPI_ResetNSS(MySPI_Context_t * ctx)
{
    RETURN_ON_NULL(ctx, VOID);
    RETURN_ON_NULL(ctx->nss_port, VOID);

    LL_GPIO_SetOutputPin(ctx->nss_port, ctx->nss_pin);
}

u8 MySPI_Transfer(MySPI_Context_t * ctx, u8 byte)
{
    RETURN_ON_NULL(ctx, 0);
    RETURN_ON_NULL(ctx->periph, 0);

    LL_SPI_Enable(ctx->periph);
    while(!LL_SPI_IsActiveFlag_TXE(ctx->periph))
        ;
    LL_SPI_TransmitData8(ctx->periph, byte);

    while(!LL_SPI_IsActiveFlag_RXNE(ctx->periph))
        ;
    u8 recvValue = LL_SPI_ReceiveData8(ctx->periph);
    return recvValue;
}

void MySPI_TransferArray(MySPI_Context_t * ctx, u8 * buffer, size_t count)
{
    RETURN_ON_NULL(ctx, VOID);
    RETURN_ON_NULL(ctx->periph, VOID);

    LL_SPI_Enable(ctx->periph);
    while(count > 0) {
        if(LL_SPI_IsActiveFlag_TXE(ctx->periph)) {
            LL_SPI_TransmitData8(ctx->periph, *buffer);
            buffer += sizeof(u8);
            count--;
        }
    }
    while(!LL_SPI_IsActiveFlag_TXE(ctx->periph))
        ;
    while(LL_SPI_IsActiveFlag_BSY(ctx->periph))
        ;
    LL_SPI_ClearFlag_OVR(ctx->periph);
}

void MySPI_ReceiveArray(MySPI_Context_t * ctx, u8 * buffer, size_t count)
{
    RETURN_ON_NULL(ctx, VOID);
    RETURN_ON_NULL(ctx->periph, VOID);

    LL_SPI_Enable(ctx->periph);

    bool txAllowed = true;
    while(count > 0) {
        if(txAllowed && LL_SPI_IsActiveFlag_TXE(ctx->periph)) {
            LL_SPI_TransmitData8(ctx->periph, 0);
            txAllowed = false;
        }

        if(LL_SPI_IsActiveFlag_RXNE(ctx->periph)) {
            *buffer = LL_SPI_ReceiveData8(ctx->periph);
            buffer += sizeof(u8);
            count--;
            txAllowed = true;
        }
    }
}

//
// void MySPI_InitContext( MySPI_Context_t * ctx, SPI_HandleTypeDef * hspi,
//    GPIO_TypeDef * nssPort, uint32_t nssPin )
//{
//    FillZero( ctx, sizeof( MySPI_Context_t ) );
//    ctx->hspi    = hspi;
//    ctx->nssPort = nssPort;
//    ctx->nssPin  = nssPin;
//}
//
// void MySPI_SetNSSState( MySPI_Context_t * ctx, bool state )
//{
//    HAL_GPIO_WritePin( ctx->nssPort, ctx->nssPin, state ? SET : RESET );
//}
//
// void MySPI_NSSSet( MySPI_Context_t * ctx )
//{
//    MySPI_SetNSSState( ctx, false );
//}
//
// void MySPI_NSSReset( MySPI_Context_t * ctx )
//{
//    MySPI_SetNSSState( ctx, true );
//}
//
// MySPI_Result_t MySPI_TransmitByte(
//    MySPI_Context_t * ctx, uint8_t byte, uint32_t timeout )
//{
//    return MySPI_TransmitByteArray( ctx, &byte, 1, timeout );
//}
//
// MySPI_Result_t MySPI_TransmitByteArray( MySPI_Context_t * ctx,
//    const uint8_t * bytes, size_t byteCount, uint32_t timeout )
//{
//    uint32_t T0, T1, DeltaT;
//
//    if( !ctx->txRequest )
//    {
//        ctx->txCompleted = false;
//        ctx->txRequest   = true;
//
//        if( HAL_OK !=
//            HAL_SPI_Transmit_IT( ctx->hspi, (uint8_t *)bytes, byteCount ) )
//        {
//            ctx->txRequest = false;
//            return MySPI_Result_Error;
//        }
//    }
//
//    T0 = HAL_GetTick( );
//    do
//    {
//        if( ctx->txCompleted )
//        {
//            ctx->txRequest = false;
//            return MySPI_Result_Ok;
//        }
//
//        if( ctx->txErrorFlag )
//        {
//            ctx->txRequest = false;
//            return MySPI_Result_Error;
//        }
//
//        T1     = HAL_GetTick( );
//        DeltaT = T1 - T0;
//    } while( DeltaT < timeout );
//
//    return MySPI_Result_Timeout;
//}
//
// MySPI_Result_t MySPI_ReceiveByte(
//    MySPI_Context_t * ctx, _OUT_ uint8_t * byte, uint32_t timeout )
//{
//    return MySPI_ReceiveBytes( ctx, byte, 1, timeout );
//}
//
// MySPI_Result_t MySPI_ReceiveBytes( MySPI_Context_t * ctx, _OUT_ uint8_t * bytes,
//    size_t byteCount, uint32_t timeout )
//{
//    uint32_t T0, T1, DeltaT;
//
//    if( !ctx->rxRequest )
//    {
//        ctx->rxRequest   = true;
//        ctx->rxCompleted = false;
//
//        if( HAL_OK != HAL_SPI_Receive_IT( ctx->hspi, bytes, byteCount ) )
//        {
//            ctx->rxRequest = false;
//            return MySPI_Result_Error;
//        }
//    }
//
//    T0 = HAL_GetTick( );
//    do
//    {
//        if( ctx->rxCompleted )
//        {
//            ctx->rxRequest = false;
//            return MySPI_Result_Ok;
//        }
//
//        if( ctx->rxErrorFlag )
//        {
//            ctx->rxRequest = false;
//            return MySPI_Result_Error;
//        }
//
//        T1     = HAL_GetTick( );
//        DeltaT = T1 - T0;
//    } while( DeltaT < timeout );
//
//    return MySPI_Result_Timeout;
//}
//
//
//__weak void SPI3_MspInit( void ) {}
//__weak void SPI3_MspDeInit( void ) {}
//__weak void SPI3_TxCpltCallback( void ) {}
//__weak void SPI3_RxCpltCallback( void ) {}
//
// void HAL_SPI_MspInit( SPI_HandleTypeDef * hspi )
//{
//    if( hspi->Instance == SPI3 )
//    {
//        SPI3_MspInit( );
//    }
//}
//
// void HAL_SPI_MspDeInit( SPI_HandleTypeDef * hspi )
//{
//    if( hspi->Instance == SPI3 )
//    {
//        SPI3_MspDeInit( );
//    }
//}
//
// void HAL_SPI_TxCpltCallback( SPI_HandleTypeDef * hspi )
//{
//    if( hspi->Instance == SPI3 )
//    {
//        SPI3_TxCpltCallback( );
//    }
//}
//
// void HAL_SPI_RxCpltCallback( SPI_HandleTypeDef * hspi )
//{
//    if( hspi->Instance == SPI3 )
//    {
//        SPI3_RxCpltCallback( );
//    }
//}
