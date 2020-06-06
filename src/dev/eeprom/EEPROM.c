///*****************************************************************************
// * @file        EEPROM.h
// * @brief       EEPROM high level driver
// * @author      gruenzinger.stefan@gmail.com
// * @copyright   Copyright (c) 2020 Stefan Grünzinger
// *****************************************************************************
// */
//
//#include "dev/eeprom/EEPROM.h"
//
//#include "Common.h"
//#include "dev/eeprom/EEPROM_LowLevel.h"
//#include "security/SHA512.h"
//#include "system/timing.h"
//#include "utils/memory.h"
//
///**
// * Initialization data is stored on the EEPROM at address 0x000000
// * The data is secured by a sha-512 checksum
// *
// * Layout:
// *  offset      | description
// * -------------+-----------------
// *  0000 - 0003 | Magic Word 1
// *  0004 - 0007 | Magic Word 2
// *  0008 - 2055 | Data
// *  2056 - 2119 | Hash
// */
//
//
//#define EEPROM_InitData_Begin       0x0000
//#define EEPROM_InitData_MagicLength 8
//#define EEPROM_InitData_DataBegin \
//    ( EEPROM_InitData_Begin + EEPROM_InitData_MagicLength )
//#define EEPROM_InitData_DataLength 2048
//#define EEPROM_InitData_ChecksumBegin \
//    ( EEPROM_InitDataBegin + EEPROM_InitDataLength )
//#define EEPROM_InitData_ChecksumLength 64 // SHA512 -> 512 bits
//#define EEPROM_InitData_Length                                   \
//    ( EEPROM_InitData_MagicLength + EEPROM_InitData_DataLength + \
//        EEPROM_InitData_ChecksumLength )
//
//#define EEPROM_InitData_Magic1 0x784b1f25
//#define EEPROM_InitData_Magic2 0x98e38a4e
//
//#define BlockSize 256
//#define _R( x )       \
//    if( !( x ) )      \
//    {                 \
//        return false; \
//    }
//
//typedef struct
//{
//    uint32_t MagicWord1;
//    uint32_t MagicWord2;
//    uint8_t  Data[ EEPROM_InitData_DataLength ];
//    uint8_t  Hash[ EEPROM_InitData_ChecksumLength ];
//} EEPROM_InitDataStruct_t;
//
//typedef union
//{
//    EEPROM_InitDataStruct_t Struct;
//    uint32_t                Words[ EEPROM_InitData_Length / 4 ];
//    uint8_t                 Bytes[ EEPROM_InitData_Length ];
//} EEPROM_InitData_t;
//
//bool EEPROM_WriteBytes(
//    uint16_t address, const uint8_t * bytes, uint32_t byteCount )
//{
//    int32_t  remainingBytes = byteCount;
//    uint32_t byteOffset     = 0;
//
//    _R( EEPROM_LowLevel_EnableWrite( ) );
//    _R( EEPROM_LowLevel_WaitForStatusFlag( 2, 2, 500 ) );
//
//    while( remainingBytes > 0 )
//    {
//        _R( EEPROM_LowLevel_WaitForStatusFlag(
//            1, 0, 500 ) );                    // check for busy bit
//        _R( EEPROM_LowLevel_EnableWrite( ) ); // Enable write latch
//        _R( EEPROM_LowLevel_WaitForStatusFlag(
//            2, 2, 500 ) ); // check for write-enable bit
//        _R( EEPROM_LowLevel_WriteBytes( address + byteOffset,
//            bytes + byteOffset,
//            ( remainingBytes > BlockSize ) ? BlockSize : remainingBytes ) );
//
//        remainingBytes -= BlockSize;
//        byteOffset += BlockSize;
//    }
//
//    _R( EEPROM_LowLevel_WaitForStatusFlag( 1, 0, 500 ) ); // check for busy bit
//    _R( EEPROM_LowLevel_DisableWrite( ) );
//    _R( EEPROM_LowLevel_WaitForStatusFlag( 2, 0, 500 ) );
//
//    return true;
//}
//
//bool EEPROM_ReadBytes(
//    uint16_t address, uint8_t * outBytes, uint32_t byteCount )
//{
//    int32_t  remainingBytes = byteCount;
//    uint32_t byteOffset     = 0;
//
//    while( remainingBytes > 0 )
//    {
//        _R( EEPROM_LowLevel_WaitForStatusFlag( 1, 0, 500 ) );
//        _R( EEPROM_LowLevel_ReadBytes( address + byteOffset,
//            outBytes + byteOffset,
//            ( remainingBytes > BlockSize ) ? BlockSize : remainingBytes ) );
//
//        remainingBytes -= BlockSize;
//        byteOffset += BlockSize;
//    }
//
//    return true;
//}
//
//bool EEPROM_TryGetInitData(
//    uint8_t initData[ static EEPROM_InitData_DataLength ] )
//{
//    EEPROM_InitData_t   initDataStruct = { 0 };
//    SHA512_ContextVar_t SHA512Context  = { 0 };
//
//    FillZero( initData, EEPROM_InitData_DataLength );
//
//    _R( EEPROM_ReadBytes(
//        EEPROM_InitData_Begin, initDataStruct.Bytes, EEPROM_InitData_Length ) );
//
//    if( initDataStruct.Struct.MagicWord1 != EEPROM_InitData_Magic1 ||
//        initDataStruct.Struct.MagicWord2 != EEPROM_InitData_Magic2 )
//    {
//        return false;
//    }
//
//    uint8_t ComputedHash[ EEPROM_InitData_ChecksumLength ];
//
//    SHA512_Init( &SHA512Context );
//    SHA512_AddBytes( &SHA512Context, initDataStruct.Bytes,
//        EEPROM_InitData_MagicLength + EEPROM_InitData_DataLength );
//    SHA512_Finish( &SHA512Context );
//    SHA512_GetResultFirstBytes(
//        &SHA512Context, ComputedHash, EEPROM_InitData_ChecksumLength );
//
//    for( int i = 0; i < EEPROM_InitData_ChecksumLength; i++ )
//    {
//        if( initDataStruct.Struct.Hash[ i ] != ComputedHash[ i ] )
//        {
//            return false;
//        }
//    }
//
//    memcpy( initData, initDataStruct.Struct.Data, EEPROM_InitData_DataLength );
//
//    return true;
//}
//
//
//bool EEPROM_SetInitData( uint8_t initData[ static EEPROM_InitData_DataLength ] )
//{
//    EEPROM_InitData_t   initDataStruct   = { 0 };
//    EEPROM_InitData_t   initDataReadback = { 0 };
//    SHA512_ContextVar_t SHA512Context    = { 0 };
//
//    initDataStruct.Struct.MagicWord1 = EEPROM_InitData_Magic1;
//    initDataStruct.Struct.MagicWord2 = EEPROM_InitData_Magic2;
//
//    for( int i = 0; i < EEPROM_InitData_DataLength; i++ )
//    {
//        initDataStruct.Struct.Data[ i ] = initData[ i ];
//    }
//
//    SHA512_Init( &SHA512Context );
//    SHA512_AddBytes( &SHA512Context, initDataStruct.Bytes,
//        EEPROM_InitData_MagicLength + EEPROM_InitData_DataLength );
//    SHA512_Finish( &SHA512Context );
//    SHA512_GetResultFirstBytes( &SHA512Context, initDataStruct.Struct.Hash,
//        EEPROM_InitData_ChecksumLength );
//
//    _R( EEPROM_WriteBytes(
//        EEPROM_InitData_Begin, initDataStruct.Bytes, EEPROM_InitData_Length ) );
//    _R( EEPROM_ReadBytes( EEPROM_InitData_Begin, initDataReadback.Bytes,
//        EEPROM_InitData_Length ) );
//
//    for( int i = 0; i < EEPROM_InitData_Length; i++ )
//    {
//        if( initDataStruct.Bytes[ i ] != initDataReadback.Bytes[ i ] )
//        {
//            return false;
//        }
//    }
//
//    return true;
//}
