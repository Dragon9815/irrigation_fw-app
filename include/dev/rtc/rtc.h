#ifndef __DEV_RTC__
#define __DEV_RTC__

#include "Common.h"
#include "types.h"

#include "utils/datetime.h"

extern void rtc_initialize(void);
extern void rtc_getCurrentDateTime(datetime_t * out_datetime);
extern void rtc_setCurrentDateTime(const datetime_t * datetime);

#endif /* __DEV_RTC__ */