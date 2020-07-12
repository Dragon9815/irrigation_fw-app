/*****************************************************************************
 * @file        datetime.c
 * @brief       Date and Time calculations and interface
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "utils/datetime.h"

#include "dev/rtc/rtc.h"
static const int HOUR_MINUTES      = 60;
static const int DAY_MINUTES       = 24 * HOUR_MINUTES;
static const int MONTH_MINUTES[13] = {
    0,
    DAY_MINUTES * 31,
    DAY_MINUTES *(31 + 28),
    DAY_MINUTES *(31 + 28 + 31),
    DAY_MINUTES *(31 + 28 + 31 + 30),
    DAY_MINUTES *(31 + 28 + 31 + 30 + 31),
    DAY_MINUTES *(31 + 28 + 31 + 30 + 31 + 30),
    DAY_MINUTES *(31 + 28 + 31 + 30 + 31 + 30 + 31),
    DAY_MINUTES *(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31),
    DAY_MINUTES *(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
    DAY_MINUTES *(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),
    DAY_MINUTES *(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30),
    DAY_MINUTES *(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31),
};
static const int YEAR_MINUTES = 365 * DAY_MINUTES;

static u8 _bcdToByte(u8 value)
{
    u32 tmp = 0;

    tmp = ((u8)(value & 0xF0) >> 4) * 10;
    return (tmp + (value & 0x0F));
}

static u8 _byteToBcd(u8 value)
{
    uint32_t bcdhigh = 0U;

    while(value >= 10U) {
        bcdhigh++;
        value -= 10U;
    }

    return ((uint8_t)(bcdhigh << 4U) | value);
}

datetime_t datetime_now(void)
{
    datetime_t result = { 0 };

    rtc_getCurrentDateTime(&result);

    return result;
}

datetime_t datetime_fromTime(u8 hour, u8 minute, u8 second)
{
    datetime_t result = { 0 };

    result.hour   = hour;
    result.minute = minute;
    result.second = second;

    return result;
}

datetime_t datetime_toFormat(const datetime_t * datetime, const datetime_format_t format)
{
    datetime_t result = { 0 };

    if(datetime->format == format) {
        return *datetime;
    }

    result.format = format;
    if(format == DTFORMAT_BIN) {
        result.year   = _bcdToByte(datetime->year);
        result.month  = _bcdToByte(datetime->month);
        result.day    = _bcdToByte(datetime->day);
        result.hour   = _bcdToByte(datetime->hour);
        result.minute = _bcdToByte(datetime->minute);
        result.second = _bcdToByte(datetime->second);
    }
    else {
        result.year   = _byteToBcd(datetime->year);
        result.month  = _byteToBcd(datetime->month);
        result.day    = _byteToBcd(datetime->day);
        result.hour   = _byteToBcd(datetime->hour);
        result.minute = _byteToBcd(datetime->minute);
        result.second = _byteToBcd(datetime->second);
    }

    return result;
}

static bool isLeapYear(int year)
{
    return (((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0);
}

uint32_t datetime_getMinutes(const datetime_t * datetime)
{
    uint32_t   minutes;
    datetime_t dt = datetime_toFormat(datetime, DTFORMAT_BIN);

    minutes = YEAR_MINUTES * dt.year;
    minutes += (dt.year / 4) * DAY_MINUTES; // adjust for past leap years
    minutes += MONTH_MINUTES[dt.month];

    if(!isLeapYear(dt.year) && dt.month > 2) {
        // adjust leap day of current year
        minutes -= DAY_MINUTES;
    }

    minutes += DAY_MINUTES * dt.day;
    minutes += HOUR_MINUTES * dt.hour;
    minutes += dt.minute;

    return minutes;
}

uint32_t datetime_getMinutesToday(const datetime_t * datetime)
{
    datetime_t dtBin = datetime_toFormat(datetime, DTFORMAT_BIN);
    return (dtBin.hour * 60) + dtBin.minute;
}

uint32_t datetime_getHoursToday(const datetime_t * datetime)
{
    datetime_t dtBin = datetime_toFormat(datetime, DTFORMAT_BIN);
    return dtBin.hour;
}

static uint32_t diffu32(uint32_t a, uint32_t b)
{
    if(a > b) {
        return a - b;
    }
    else {
        return b - a;
    }
}

uint32_t datetime_diffMinutes(const datetime_t * t1, const datetime_t * t2)
{
    uint32_t min1 = datetime_getMinutesToday(t1);
    uint32_t min2 = datetime_getMinutesToday(t2);
    return diffu32(min1, min2);
}

uint32_t datetime_diffHours(const datetime_t * t1, const datetime_t * t2)
{
    uint32_t hour1 = datetime_getMinutes(t1) / 60;
    uint32_t hour2 = datetime_getMinutes(t2) / 60;
    return diffu32(hour1, hour2);
}
