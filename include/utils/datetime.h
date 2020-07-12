/*****************************************************************************
 * @file        datetime.h
 * @brief       Date and Time calculations and interface
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __DATETIME_H__
#define __DATETIME_H__

#include "Common.h"

typedef enum
{
    DTFORMAT_BIN = 0,
    DTFORMAT_BCD,
} datetime_format_t;

typedef struct
{
    datetime_format_t format;

    u16 year;
    u8  month;
    u8  day;

    u8 hour;
    u8 minute;
    u8 second;
} datetime_t;

extern datetime_t datetime_now(void);
extern datetime_t datetime_fromTime(u8 hour, u8 minute, u8 second);
extern datetime_t datetime_toFormat(const datetime_t * datetime, const datetime_format_t format);

extern uint32_t datetime_getMinutes(const datetime_t * datetime);

extern uint32_t datetime_getMinutesToday(const datetime_t * datetime);
extern uint32_t datetime_getHoursToday(const datetime_t * datetime);

//extern uint32_t datetime_diffSeconds(const datetime_t* t1, const datetime_t* t2);
extern uint32_t datetime_diffMinutes(const datetime_t * t1, const datetime_t * t2);
extern uint32_t datetime_diffHours(const datetime_t * t1, const datetime_t * t2);

#endif /* __DATETIME_H__ */
