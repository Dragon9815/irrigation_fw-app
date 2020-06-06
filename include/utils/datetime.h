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
    DTFORMAT_BCD,
    DTFORMAT_BIN
} datetime_format_t;

typedef struct
{
    datetime_format_t format;

    uint16_t year;
    uint8_t  month;
    uint8_t  day;

    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} datetime_t;

extern void datetime_now(datetime_t * out_datetime);

//extern uint32_t datetime_getSecondsOfDay(const datetime_t* datetime);
extern uint32_t datetime_getMinutesToday(const datetime_t * datetime);
extern uint32_t datetime_getHoursToday(const datetime_t * datetime);

//extern uint32_t datetime_diffSeconds(const datetime_t* t1, const datetime_t* t2);
extern uint32_t datetime_diffMinutes(const datetime_t * t1, const datetime_t * t2);
extern uint32_t datetime_diffHours(const datetime_t * t1, const datetime_t * t2);

#endif /* __DATETIME_H__ */
