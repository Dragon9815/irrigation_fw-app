///*****************************************************************************
// * @file        EEPROM_LowLevel.c
// * @brief       EEPROM LowLevel driver
// * @author      gruenzinger.stefan@gmail.com
// * @copyright   Copyright (c) 2020 Stefan Grünzinger
// *****************************************************************************
// */
//
//#include "dev/eeprom/EEPROM_LowLevel.h"
//
//#include "Ident.h"
//#include "Common.h"
//#include "dev/spi/SPI_EEPROM.h"
//
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//// AT25M1 EEPROM  protocol
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//#define Command_WREN  0x06 // Set Write Enable Latch
//#define Command_WRDI  0x04 // Reset Write Enable Latch
//#define Command_RDSR  0x05 // Read Status Register
//#define Command_WRSR  0x01 // Write Status Register
//#define Command_READ  0x03 // Read data from memory array
//#define Command_WRITE 0x02 // Write data to memory array
//
//#define Flag_Busy_Mask       0x01
//#define Flag_Busy_Match      0x00
//#define Flag_Writeable_Mask  0x02
//#define Flag_Writeable_Match 0x02
//
// const uint8_t           EEPROM_LowLevel_TxData[ 1 ];
// EEPROM_LowLevel_State_t EEPROM_LowLevel_State;
//
// volatile uint32_t EEPROM_SoftSPI_Delay_Timer;
//
// void EEPROM_LowLevel_Delay_us( uint32_t x )
//{
//    EEPROM_SoftSPI_Delay_Timer = 0;
//    while( EEPROM_SoftSPI_Delay_Timer < x )
//    {
//        EEPROM_SoftSPI_Delay_Timer++;
//    }
//}
//
// void EEPROM_LowLevel_Initialize( void )
//{
//    SPI_EEPROM_Initalize( );
//}
//
// bool EEPROM_LowLevel_ReadStatusRegister( void )
//{
//    MySPI_NSSSet( &SPI_EEPROM_Context );
//    MySPI_Result_t result =
//        MySPI_TransmitByte( &SPI_EEPROM_Context, Command_RDSR, 100 );
//
//    if( MySPI_Result_Ok != result )
//    {
//        MySPI_NSSReset( &SPI_EEPROM_Context );
//        return false;
//    }
//
//    result = MySPI_ReceiveByte( &SPI_EEPROM_Context,
//        &EEPROM_LowLevel_State.ChipStatusRegisterValue, 100 );
//    MySPI_NSSReset( &SPI_EEPROM_Context );
//
//    return MySPI_Result_Ok == result;
//}
//
// bool EEPROM_LowLevel_WaitForStatusFlag(
//    uint8_t mask, uint8_t match, uint32_t timeout )
//{
//    uint32_t T0 = HAL_GetTick( );
//    uint32_t T1, dT;
//    do
//    {
//        MySPI_Result_t result = EEPROM_LowLevel_ReadStatusRegister( );
//
//        if( result == MySPI_Result_Error )
//        {
//            EEPROM_LowLevel_State.ErrorFlag = true;
//            return false;
//        }
//
//        T1 = HAL_GetTick( );
//        dT = T1 - T0;
//    } while(
//        ( EEPROM_LowLevel_State.ChipStatusRegisterValue & mask ) != match &&
//        dT < timeout );
//
//    return dT < timeout;
//}
//
//
// bool EEPROM_LowLevel_EnableWrite( void )
//{
//    MySPI_NSSSet( &SPI_EEPROM_Context );
//    MySPI_Result_t result =
//        MySPI_TransmitByte( &SPI_EEPROM_Context, Command_WREN, 100 );
//    MySPI_NSSReset( &SPI_EEPROM_Context );
//
//    return MySPI_Result_Ok == result;
//}
//
// bool EEPROM_LowLevel_DisableWrite( void )
//{
//    MySPI_NSSSet( &SPI_EEPROM_Context );
//    MySPI_Result_t result =
//        MySPI_TransmitByte( &SPI_EEPROM_Context, Command_WRDI, 100 );
//    MySPI_NSSReset( &SPI_EEPROM_Context );
//
//    return MySPI_Result_Ok == result;
//}
//
// bool EEPROM_LowLevel_TransmitAddress( uint32_t EEPROMByteAddress )
//{
//    MySPI_Result_t result;
//
//    result = MySPI_TransmitByte(
//        &SPI_EEPROM_Context, ( EEPROMByteAddress >> 16 ) & 0xFF, 100 );
//    if( MySPI_Result_Ok != result )
//    {
//        return false;
//    }
//
//    result = MySPI_TransmitByte(
//        &SPI_EEPROM_Context, ( EEPROMByteAddress >> 8 ) & 0xFF, 100 );
//    if( MySPI_Result_Ok != result )
//    {
//        return false;
//    }
//
//    result = MySPI_TransmitByte(
//        &SPI_EEPROM_Context, ( EEPROMByteAddress >> 0 ) & 0xFF, 100 );
//    if( MySPI_Result_Ok != result )
//    {
//        return false;
//    }
//
//    return true;
//}
//
// bool EEPROM_LowLevel_WriteBytes(
//    uint32_t EEPROMByteAddress, const uint8_t DataBytes[], int DataBytesCount )
//{
//    MySPI_Result_t result;
//
//    if( DataBytesCount > 256 )
//    {
//        DataBytesCount = 256;
//    }
//
//    MySPI_NSSSet( &SPI_EEPROM_Context );
//    result = MySPI_TransmitByte( &SPI_EEPROM_Context, Command_WRITE, 100 );
//    if( MySPI_Result_Ok != result )
//    {
//        MySPI_NSSReset( &SPI_EEPROM_Context );
//        return false;
//    }
//
//    if( !EEPROM_LowLevel_TransmitAddress( EEPROMByteAddress ) )
//    {
//        MySPI_NSSReset( &SPI_EEPROM_Context );
//        return false;
//    }
//
//    result = MySPI_TransmitByteArray(
//        &SPI_EEPROM_Context, DataBytes, DataBytesCount, 100 );
//    MySPI_NSSReset( &SPI_EEPROM_Context );
//
//    return MySPI_Result_Ok == result;
//}
//
// bool EEPROM_LowLevel_ReadBytes(
//    uint32_t EEPROMByteAddress, _OUT_ uint8_t DataBytes[], int DataBytesCount )
//{
//    MySPI_Result_t result;
//
//    if( DataBytesCount > 256 )
//    {
//        DataBytesCount = 256;
//    }
//
//    MySPI_NSSSet( &SPI_EEPROM_Context );
//    result = MySPI_TransmitByte( &SPI_EEPROM_Context, Command_READ, 100 );
//    if( MySPI_Result_Ok != result )
//    {
//        MySPI_NSSReset( &SPI_EEPROM_Context );
//        return false;
//    }
//
//    if( !EEPROM_LowLevel_TransmitAddress( EEPROMByteAddress ) )
//    {
//        MySPI_NSSReset( &SPI_EEPROM_Context );
//        return false;
//    }
//
//    //	for( int i = 0 ; i < DataBytesCount ; i++ )
//    //	{
//    //		MySPI_NSSSet( &SPI_EEPROM_Context );
//    //		result = MySPI_ReceiveByte( &SPI_EEPROM_Context, &DataBytes[ i
//    //], 100
//    //); 		MySPI_NSSReset( &SPI_EEPROM_Context ); 		if( MySPI_Result_Ok
//    //!= result )
//    //		{
//    //			return false;
//    //		}
//    //		EEPROM_LowLevel_Delay_us( 1000 );
//    //	}
//
//    result = MySPI_ReceiveBytes(
//        &SPI_EEPROM_Context, DataBytes, DataBytesCount, 100 );
//    MySPI_NSSReset( &SPI_EEPROM_Context );
//
//    return MySPI_Result_Ok == result;
//}
//
//
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//// Published Functions
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//
//// uint32_t EEPROM_Thread_T0;
//// uint32_t EEPROM_Thread_T1;
//// uint32_t EEPROM_Thread_DeltaT;
//// uint32_t EEPROM_Thread_Action;
// uint8_t EEPROM_Readback_Buffer[ 4 ];
////
//#define EEPROM_ByteAddress 0x000000
//#define EEPROM_Timeout     500
////
// uint8_t EEPROM_Write_Buffer[ 4 ];
//
// void EEPROM_Init_WriteEEPROM( void )
//{
//    /* Working example: */
//
//    //	uint8_t b = 0x58;
//    //
//    //	EEPROM_Write_Buffer[ 0 ] = b;
//    //	EEPROM_Write_Buffer[ 1 ] = ~b;
//    //	EEPROM_Write_Buffer[ 2 ] = b + 1;
//    //	EEPROM_Write_Buffer[ 3 ] = b - 1;
//    //
//    //	if( ! EEPROM_LowLevel_EnableWrite( ) )
//    //	{
//    //		return; // TODO: Report error
//    //	}
//    //
//    //	if( ! EEPROM_LowLevel_WaitForStatusFlag( 2, 2, EEPROM_Timeout ) )
//    //	{
//    //		return; // TODO: Report error
//    //	}
//    //
//    //	if( ! EEPROM_LowLevel_WriteBytes( EEPROM_ByteAddress,
//    // EEPROM_Write_Buffer, 4 ) )
//    //	{
//    //		return; // TODO: Report error
//    //	}
//    //
//    //	if( ! EEPROM_LowLevel_WaitForStatusFlag( 1, 0, EEPROM_Timeout ) )
//    //	{
//    //		return; // TODO: Report error
//    //	}
//    //
//    //	if( ! EEPROM_LowLevel_DisableWrite( ) )
//    //	{
//    //		return; // TODO: Report error
//    //	}
//    //
//    //	if( ! EEPROM_LowLevel_ReadStatusRegister( ) )
//    //	{
//    //		return; // TODO: Report error
//    //	}
//    //
//    //	if( ! EEPROM_LowLevel_WaitForStatusFlag( 2, 0, EEPROM_Timeout ) )
//    //	{
//    //		return; // TODO: Report error
//    //	}
//    //
//    //	if( ! EEPROM_LowLevel_ReadBytes( EEPROM_ByteAddress,
//    // EEPROM_Readback_Buffer, 4 ) )
//    //	{
//    //		return; // TODO: Report error
//    //	}
//}
////#endif
//
//
// void EEPROM_Init_ReadEEPROM( void )
//{
//    //    EEPROM_LowLevel_ReadStatusRegister( );
//    //
//    //    EEPROM_LowLevel_ReadBytes( EEPROM_ByteAddress, EEPROM_Readback_Buffer,
//    //    4 ); if( 1 <= EEPROM_Readback_Buffer[ 0 ] && EEPROM_Readback_Buffer[ 0
//    //    ] <= 36 )
//    //    {
//    //        if( ( EEPROM_Readback_Buffer[ 0 ] ^ 0xFF ) ==
//    //        EEPROM_Readback_Buffer[ 1 ] &&
//    //            ( EEPROM_Readback_Buffer[ 0 ] + 1 ) == EEPROM_Readback_Buffer[
//    //            2 ] && ( EEPROM_Readback_Buffer[ 0 ] - 1 ) ==
//    //            EEPROM_Readback_Buffer[ 3 ] )
//    //        {
//    //            DeviceIdent.CAN_NodeAddress = EEPROM_Readback_Buffer[ 0 ];
//    //        }
//    //    }
//    //    Delay_ms( 10 );
//}
//
//
// void EEPROM_Init( void )
//{
//    EEPROM_LowLevel_Initialize( );
//
//    //	EEPROM_Init_WriteEEPROM( );
//    //    EEPROM_Init_ReadEEPROM( );
//
//    //    EEPROM_Thread_T0     = GetTickCount_ms();
//    //    EEPROM_Thread_Action = 0;
//}
//
//
// void EEPROM_ExecuteThread( void )
//{
//    //    EEPROM_Thread_T1     = GetTickCount_ms();
//    //    EEPROM_Thread_DeltaT = EEPROM_Thread_T1 - EEPROM_Thread_T0;
//    //    if( EEPROM_Thread_DeltaT < 200 )
//    //    {
//    //        return;
//    //    }
//    //    EEPROM_Thread_T0     = EEPROM_Thread_T1;
//    //
//    //    if( 0 == ( EEPROM_Thread_Action & 1 ) )
//    //    {
//    //        // EEPROM_LowLevel_EnableWrite( );
//    //    }
//    //    else
//    //    {
//    //        EEPROM_LowLevel_ReadStatusRegister( );
//    //    }
//    //
//    //    EEPROM_Thread_Action ++;
//}
