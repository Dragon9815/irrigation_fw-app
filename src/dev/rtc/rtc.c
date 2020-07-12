#include "dev/rtc/rtc.h"

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_rcc.h>
#include <stm32f4xx_ll_rtc.h>

void rtc_initialize(void)
{
    LL_RTC_InitTypeDef RTC_InitStruct = { 0 };

    LL_RCC_EnableRTC();

    RTC_InitStruct.HourFormat      = LL_RTC_HOURFORMAT_24HOUR;
    RTC_InitStruct.AsynchPrescaler = 127;
    RTC_InitStruct.SynchPrescaler  = 255;
    LL_RTC_Init(RTC, &RTC_InitStruct);
    LL_RTC_SetAsynchPrescaler(RTC, 127);
    LL_RTC_SetSynchPrescaler(RTC, 255);
}

void rtc_getCurrentDateTime(datetime_t * out_datetime)
{
    uint32_t time = LL_RTC_TIME_Get(RTC);
    uint32_t date = LL_RTC_DATE_Get(RTC);

    out_datetime->year   = date & 0xFF;
    out_datetime->month  = (date >> RTC_OFFSET_MONTH) & 0xFF;
    out_datetime->day    = (date >> RTC_OFFSET_DAY) & 0xFF;
    out_datetime->hour   = (time >> RTC_OFFSET_HOUR) & 0xFF;
    out_datetime->minute = (time >> RTC_OFFSET_MINUTE) & 0xFF;
    out_datetime->second = time & 0xFF;
    out_datetime->format = DTFORMAT_BCD;
}

void rtc_setCurrentDateTime(const datetime_t * datetime)
{
    datetime_t dtBCD = datetime_toFormat(datetime, DTFORMAT_BCD);

    LL_RTC_DateTypeDef date;
    date.Year  = dtBCD.year;
    date.Month = dtBCD.month;
    date.Day   = dtBCD.day;
    LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BCD, &date);

    LL_RTC_TimeTypeDef time;
    time.Hours      = dtBCD.hour;
    time.Minutes    = dtBCD.minute;
    time.Seconds    = dtBCD.minute;
    time.TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
    LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BCD, &time);
}