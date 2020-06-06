/*****************************************************************************
 */

//---------------------------------------------------------------------------------------
// CRC-32C --> Castagnoli
//
// See iSCSI specification
//---------------------------------------------------------------------------------------

#ifndef CRC32C_H
#define CRC32C_H

#include "Common.h"


// CRC-32C Calculation

__SG_BEGIN_DECLS

uint32_t CRC32C_ComputeBlock( const uint8_t * aAddress, uint32_t aLength );
uint32_t CRC32C_StartComputeBlock( const uint8_t * aAddress, uint32_t aLength );
uint32_t CRC32C_StartByte( const uint8_t aByte );
uint32_t CRC32C_AddByte( uint32_t aCRC, uint8_t aByte );
uint32_t CRC32C_EndComputeBlock( uint32_t aCRC );

__SG_END_DECLS

#endif
