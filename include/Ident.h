/*****************************************************************************
 * @file        Ident.h
 * @brief       MCU identification info
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __IDENT_H__
#define __IDENT_H__

#include "Common.h"

#define CAN_NodeId 0x3D

__SG_BEGIN_DECLS

typedef struct
{
    uint16_t Major;
    uint16_t Minor;
    uint16_t Subminor;
} Version_t;

typedef struct
{
    Version_t FWVersion;
    Version_t BLVersion;
    uint16_t  HWVersion;

    uint32_t SerialNumber;
    uint16_t Revision;

    uint32_t ProductCode;
    uint32_t AdditionalInfo;

    uint32_t MCU_UniqueId_Word0;
    uint32_t MCU_UniqueId_Word1;
    uint32_t MCU_UniqueId_Word2;
    uint32_t MCU_ChipTypeId;
    uint16_t MCU_FlashSize;
} Ident_t;

extern Ident_t Ident;

__SG_END_DECLS

#endif /* __IDENT_H__ */
