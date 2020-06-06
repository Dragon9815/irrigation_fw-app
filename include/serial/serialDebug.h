/*****************************************************************************
 * @file        serialDebug.h
 * @brief       Serial interface used for debugging
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __SERIAL_DEBUG_H__
#define __SERIAL_DEBUG_H__

#include "types.h"

/*****************************************************************************
 * @brief  Intialize the serial debug interface.
 * 
 * @author Stefan Grünzinger
 * @rev    1
 *****************************************************************************/
extern void serialDebug_initialize(void);

/*****************************************************************************
 * @brief  Send one single byte.
 * 
 * @param  byte The byte to send.
 *
 * @author Stefan Grünzinger
 * @rev    1
 *****************************************************************************/
extern void serialDebug_sendByte(u8 byte);

/*****************************************************************************
 * @brief  Send an array of bytes.
 *
 * @param  bytes The array of bytes to send.
 * @param  count The amount of bytes to send.
 * 
 * @author Stefan Grünzinger
 * @rev    1
 *****************************************************************************/
extern void serialDebug_sendBytes(const u8 * bytes, u32 count);

/*****************************************************************************
 * @brief  Sends a zero terminated string.
 * 
 * @param  str The zero terminated string to send.
 *
 * @author Stefan Grünzinger
 * @rev    1
 *****************************************************************************/
extern void serialDebug_sendString(const char * str);

/*****************************************************************************
 * @brief  Try to read an amount of bytes from the receive buffer.
 *
 * @param  buffer The buffer to store the read bytes in.
 * @param  count  The amount of bytes to read.
 *
 * @return The amount of bytes actually read, ranging from 0 to range.
 *
 * @author Stefan Grünzinger
 * @rev    1
 *****************************************************************************/
extern u32 serialDebug_readBytes(u8 * buffer, u32 count);

#endif /* __SERIAL_DEBUG_H__ */
