/*****************************************************************************
 * @file        EEPROM_LowLevel.h
 * @brief       EEPROM LowLevel driver
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __EEPROM_LOWLEVEL_H__
#define __EEPROM_LOWLEVEL_H__

#include "Common.h"

typedef enum
{
    EEPROM_Error_Write_Timeout, // Waiting for flags while writing timed out
    EEPROM_Error_Read_Timeout,  // Waiting for flags while reading timed out
    EEPROM_Error_Read_Only      // Tried to write to a read only area
} EEPROM_Error_t;

typedef struct
{
    uint8_t ChipStatusRegisterValue;
    bool    ErrorFlag;
} EEPROM_LowLevel_State_t;

extern EEPROM_LowLevel_State_t EEPROM_LowLevel_State;

extern void EEPROM_Init(void);
extern void EEPROM_ExecuteThread(void);
extern void EEPROM_ReadFirstBytes(_OUT_ uint8_t * Bytes[], int BytesCount);
extern void EEPROM_WriteFirstBytes(_OUT_ uint8_t * Bytes[], int BytesCount);

extern void EEPROM_LowLevel_Initialize(void);
extern bool EEPROM_LowLevel_ReadStatusRegister(void);
extern bool EEPROM_LowLevel_WaitForStatusFlag(uint8_t mask, uint8_t match, uint32_t timeout);
extern bool EEPROM_LowLevel_EnableWrite(void);
extern bool EEPROM_LowLevel_DisableWrite(void);
extern bool EEPROM_LowLevel_TransmitAddress(uint32_t EEPROMByteAddress);
extern bool EEPROM_LowLevel_WriteBytes(uint32_t EEPROMByteAddress, const uint8_t DataBytes[], int DataBytesCount);
extern bool EEPROM_LowLevel_ReadBytes(uint32_t EEPROMByteAddress, _OUT_ uint8_t DataBytes[], int DataBytesCount);

#endif // __EEPROM_LOWLEVEL_H__
