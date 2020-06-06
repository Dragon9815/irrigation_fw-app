/*****************************************************************************
 * @file        utils/memory.h
 * @brief       Memory manipulation and read functions
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __UTILS_MEMORY_H__
#define __UTILS_MEMORY_H__

#define Memory_ReadWord16(address) (*((const uint16_t *)(address)))
#define Memory_ReadWord32(address) (*((const uint32_t *)(address)))

__SG_BEGIN_DECLS

extern void FillZero(void * buffer, size_t capacity);

__SG_END_DECLS

#endif /* __UTILS_MEMORY_H__ */
