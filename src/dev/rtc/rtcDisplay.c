#include "dev/rtc/rtcDisplay.h"

#include "dev/rtc/rtc.h"
#include "dev/tm1637/tm1637.h"
#include "io/io.h"
#include "system/timing.h"
#include "utils/datetime.h"

typedef enum clockState
{
    CLKSTATE_SHOW_TIME,
    CLKSTATE_SHOW_YEAR,
    CLKSTATE_SHOW_MONTH,
    CLKSTATE_SHOW_DAY,
    CLKSTATE_SET_HOURS,
    CLKSTATE_SET_MINUTES,
    CLKSTATE_SET_DAY,
    CLKSTATE_SET_MONTH,
    CLKSTATE_SET_YEAR,
} clockState_t;

static const int MONTH_DAYS[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static clockState_t rtcDisplay_clockState;
static uint32_t     rtcDisplay_blinkT0;
static bool         rtcDisplay_blinkState;
static bool         rtcDisplay_shortPressLocked;

void rtcDisplay_initialize(void)
{
    tm1637_initialize();
    rtcDisplay_clockState       = CLKSTATE_SHOW_TIME;
    rtcDisplay_blinkT0          = 0;
    rtcDisplay_shortPressLocked = false;
    rtcDisplay_blinkState       = false;
}

static int incrementWrap(int value, int lowBound, int highBound)
{
    if(value < highBound) {
        return value + 1;
    }
    else {
        return lowBound;
    }
}

static int decrementWrap(int value, int lowBound, int highBound)
{
    if(value > lowBound) {
        return value - 1;
    }
    else {
        return highBound;
    }
}

void rtcDisplay_execThread(void)
{
    uint8_t displaybuf[4];
    bool    dotEnabled;

    uint32_t blinkT1     = Timing_GetTicks_ms();
    uint32_t blinkDeltaT = blinkT1 - rtcDisplay_blinkT0;
    if(blinkDeltaT >= 500) {
        rtcDisplay_blinkT0    = blinkT1;
        rtcDisplay_blinkState = !rtcDisplay_blinkState;
    }

    datetime_t now    = datetime_now();
    datetime_t nowBCD = datetime_toFormat(&now, DTFORMAT_BCD);
    datetime_t nowBin = datetime_toFormat(&now, DTFORMAT_BIN);

    bool dtChanged = false;

    nowBin.second = 0;

    switch(rtcDisplay_clockState) {
    case CLKSTATE_SHOW_TIME: {
        displaybuf[0] = digitToSegment[(nowBCD.hour >> 4) & 0xF];
        displaybuf[1] = digitToSegment[nowBCD.hour & 0xF];
        displaybuf[2] = digitToSegment[(nowBCD.minute >> 4) & 0xF];
        displaybuf[3] = digitToSegment[nowBCD.minute & 0xF];

        dotEnabled = rtcDisplay_blinkState;

        if(io_getInputLongPress(INPUT_CLOCK_SET)) {
            rtcDisplay_clockState = CLKSTATE_SET_HOURS;
        }
        else if(io_getInputEdgeNeg(INPUT_CLOCK_SET)) {
            if(rtcDisplay_shortPressLocked) {
                rtcDisplay_shortPressLocked = false;
            }
            else {
                rtcDisplay_clockState = CLKSTATE_SHOW_YEAR;
            }
        }
    } break;

    case CLKSTATE_SHOW_YEAR: {
        displaybuf[0] = digitToSegment[2 /*(nowBCD.year >> 12) & 0xF*/];
        displaybuf[1] = digitToSegment[0 /*(nowBCD.year >> 8) & 0xF*/];
        displaybuf[2] = digitToSegment[(nowBCD.year >> 4) & 0xF];
        displaybuf[3] = digitToSegment[nowBCD.year & 0xF];

        dotEnabled = false;

        if(io_getInputLongPress(INPUT_CLOCK_SET)) {
            rtcDisplay_clockState = CLKSTATE_SET_YEAR;
        }
        else if(io_getInputEdgeNeg(INPUT_CLOCK_SET)) {
            rtcDisplay_clockState = CLKSTATE_SHOW_MONTH;
        }
    } break;

    case CLKSTATE_SHOW_MONTH: {
        displaybuf[0] = 0b00110111;
        displaybuf[1] = 0;
        displaybuf[2] = digitToSegment[(nowBCD.month >> 4) & 0xF];
        displaybuf[3] = digitToSegment[nowBCD.month & 0xF];

        dotEnabled = false;

        if(io_getInputLongPress(INPUT_CLOCK_SET)) {
            rtcDisplay_clockState = CLKSTATE_SET_MONTH;
        }
        else if(io_getInputEdgeNeg(INPUT_CLOCK_SET)) {
            rtcDisplay_clockState = CLKSTATE_SHOW_DAY;
        }
    } break;

    case CLKSTATE_SHOW_DAY: {
        displaybuf[0] = digitToSegment[0xd];
        displaybuf[1] = 0;
        displaybuf[2] = digitToSegment[(nowBCD.day >> 4) & 0xF];
        displaybuf[3] = digitToSegment[nowBCD.day & 0xF];

        dotEnabled = false;

        if(io_getInputLongPress(INPUT_CLOCK_SET)) {
            rtcDisplay_clockState = CLKSTATE_SET_DAY;
        }
        else if(io_getInputEdgeNeg(INPUT_CLOCK_SET)) {
            rtcDisplay_clockState = CLKSTATE_SHOW_TIME;
        }
    } break;

    case CLKSTATE_SET_HOURS: {
        if(rtcDisplay_blinkState) {
            displaybuf[0] = digitToSegment[(nowBCD.hour >> 4) & 0xF];
            displaybuf[1] = digitToSegment[nowBCD.hour & 0xF];
        }
        else {
            displaybuf[0] = 0;
            displaybuf[1] = 0;
        }
        displaybuf[2] = digitToSegment[(nowBCD.minute >> 4) & 0xF];
        displaybuf[3] = digitToSegment[nowBCD.minute & 0xF];

        dotEnabled = true;

        if(io_getInputEdgePos(INPUT_CLOCK_SET)) {
            rtcDisplay_clockState = CLKSTATE_SET_MINUTES;
        }

        if(io_getInputEdgePos(INPUT_CLOCK_PLUS)) {
            nowBin.hour = incrementWrap(nowBin.hour, 0, 23);
            dtChanged   = true;
        }

        if(io_getInputEdgePos(INPUT_CLOCK_MINUS)) {
            nowBin.hour = decrementWrap(nowBin.hour, 0, 23);
            dtChanged   = true;
        }
    } break;

    case CLKSTATE_SET_MINUTES: {
        displaybuf[0] = digitToSegment[(nowBCD.hour >> 4) & 0xF];
        displaybuf[1] = digitToSegment[nowBCD.hour & 0xF];
        if(rtcDisplay_blinkState) {
            displaybuf[2] = digitToSegment[(nowBCD.minute >> 4) & 0xF];
            displaybuf[3] = digitToSegment[nowBCD.minute & 0xF];
        }
        else {
            displaybuf[2] = 0;
            displaybuf[3] = 0;
        }

        dotEnabled = true;

        if(io_getInputEdgePos(INPUT_CLOCK_SET)) {
            rtcDisplay_clockState = CLKSTATE_SET_YEAR;
        }

        if(io_getInputEdgePos(INPUT_CLOCK_PLUS)) {
            nowBin.minute = incrementWrap(nowBin.minute, 0, 59);
            dtChanged     = true;
        }

        if(io_getInputEdgePos(INPUT_CLOCK_MINUS)) {
            nowBin.minute = decrementWrap(nowBin.minute, 0, 59);
            dtChanged     = true;
        }
    } break;

    case CLKSTATE_SET_YEAR: {
        if(rtcDisplay_blinkState) {
            displaybuf[0] = digitToSegment[2 /*(nowBCD.year >> 12) & 0xF*/];
            displaybuf[1] = digitToSegment[0 /*(nowBCD.year >> 8) & 0xF*/];
            displaybuf[2] = digitToSegment[(nowBCD.year >> 4) & 0xF];
            displaybuf[3] = digitToSegment[nowBCD.year & 0xF];
        }
        else {
            displaybuf[0] = 0;
            displaybuf[1] = 0;
            displaybuf[2] = 0;
            displaybuf[3] = 0;
        }

        dotEnabled = false;

        if(io_getInputEdgePos(INPUT_CLOCK_SET)) {
            rtcDisplay_clockState = CLKSTATE_SET_MONTH;
        }

        if(io_getInputEdgePos(INPUT_CLOCK_PLUS)) {
            nowBin.year = incrementWrap(nowBin.year, 0, 99);
            dtChanged   = true;
        }

        if(io_getInputEdgePos(INPUT_CLOCK_MINUS)) {
            nowBin.year = decrementWrap(nowBin.year, 0, 99);
            dtChanged   = true;
        }
    } break;

    case CLKSTATE_SET_MONTH: {
        displaybuf[0] = 0b00110111;
        displaybuf[1] = 0;

        if(rtcDisplay_blinkState) {
            displaybuf[2] = digitToSegment[(nowBCD.month >> 4) & 0xF];
            displaybuf[3] = digitToSegment[nowBCD.month & 0xF];
        }
        else {
            displaybuf[2] = 0;
            displaybuf[3] = 0;
        }

        dotEnabled = false;

        if(io_getInputEdgePos(INPUT_CLOCK_SET)) {
            rtcDisplay_clockState = CLKSTATE_SET_DAY;
        }

        if(io_getInputEdgePos(INPUT_CLOCK_PLUS)) {
            nowBin.month = incrementWrap(nowBin.month, 1, 12);
            dtChanged    = true;
        }

        if(io_getInputEdgePos(INPUT_CLOCK_MINUS)) {
            nowBin.month = decrementWrap(nowBin.month, 1, 12);
            dtChanged    = true;
        }
    } break;

    case CLKSTATE_SET_DAY: {
        displaybuf[0] = digitToSegment[0xd];
        displaybuf[1] = 0;

        if(rtcDisplay_blinkState) {
            displaybuf[2] = digitToSegment[(nowBCD.day >> 4) & 0xF];
            displaybuf[3] = digitToSegment[nowBCD.day & 0xF];
        }
        else {
            displaybuf[2] = 0;
            displaybuf[3] = 0;
        }

        dotEnabled = false;

        if(io_getInputEdgePos(INPUT_CLOCK_SET)) {
            rtcDisplay_clockState       = CLKSTATE_SHOW_TIME;
            rtcDisplay_shortPressLocked = true;
        }

        int monthDays = MONTH_DAYS[nowBin.month];
        if(nowBin.month == 2 && (((nowBin.year % 4 == 0) && (nowBin.year % 100 != 0)) || (nowBin.year % 400 == 0))) {
            monthDays++;
        }

        if(io_getInputEdgePos(INPUT_CLOCK_PLUS)) {
            nowBin.day = incrementWrap(nowBin.day, 1, monthDays);
            dtChanged  = true;
        }

        if(io_getInputEdgePos(INPUT_CLOCK_MINUS)) {
            nowBin.day = decrementWrap(nowBin.day, 1, monthDays);
            dtChanged  = true;
        }
    } break;

    default:
        displaybuf[0]         = 0b01000000;
        displaybuf[1]         = 0b01000000;
        displaybuf[2]         = 0b01000000;
        displaybuf[3]         = 0b01000000;
        rtcDisplay_clockState = CLKSTATE_SHOW_TIME;
        break;
    }

    if(dtChanged) {
        rtcDisplay_blinkT0    = Timing_GetTicks_ms();
        rtcDisplay_blinkState = true;
        rtc_setCurrentDateTime(&nowBin);
    }

    tm1637_sendDisplayData(displaybuf, dotEnabled);
}
