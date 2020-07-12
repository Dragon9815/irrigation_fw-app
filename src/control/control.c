/*****************************************************************************
 * @file        control.c
 * @brief       Valve output processing and controlling
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "control/control.h"

#include "io/io.h"

#include <string.h>

typedef struct
{
    control_config_t config;
    control_state_t  inputState;

    bool       running;
    datetime_t runningSince;
    uint32_t   runningMinutes;
    datetime_t lastRunning;
} control_areastate_t;

#define NUM_CONTROL_AREAS 3

static control_areastate_t areaStates[NUM_CONTROL_AREAS];
static datetime_t          lastRainTime;
static datetime_t          lastWindTime;
static bool                currentlyRaining;
static bool                currentlyWindy;

static void checkLastRunning(control_areastate_t * areaState, const datetime_t now)
{
    if(areaState->lastRunning.day != now.day || areaState->lastRunning.month != now.month ||
       areaState->lastRunning.year != now.year) {
        areaState->runningMinutes = 0;
    }
}

static bool execArea(control_areastate_t * areaState)
{
    datetime_t now;
    uint32_t   minutesToday;
    uint32_t   startMinute;
    uint32_t   endMinute;

    now          = datetime_now();
    minutesToday = datetime_getMinutesToday(&now);

    switch(areaState->inputState) {
    case CONTROL_STATE_OFF:
        areaState->running = false;
        break;

    case CONTROL_STATE_AUTO:
        startMinute = datetime_getMinutesToday(&areaState->config.starttime);
        endMinute   = datetime_getMinutesToday(&areaState->config.endtime);

        if(!areaState->running) {
            checkLastRunning(areaState, now);

            uint32_t rainless = datetime_diffHours(&now, &lastRainTime);
            uint32_t windless = datetime_diffHours(&now, &lastWindTime);

            if(minutesToday >= startMinute && minutesToday < endMinute &&
               areaState->runningMinutes < areaState->config.duration_minutes && !currentlyRaining &&
               rainless >= areaState->config.rainless_hours && !currentlyWindy &&
               windless >= areaState->config.windless_hours) {
                areaState->running      = true;
                areaState->runningSince = now;
            }
        }
        else {
            areaState->lastRunning = now;
            uint32_t runningTime   = areaState->runningMinutes + datetime_diffMinutes(&now, &areaState->runningSince);
            if(minutesToday < startMinute || minutesToday >= endMinute ||
               runningTime > areaState->config.duration_minutes || currentlyRaining || currentlyWindy) {
                areaState->runningMinutes = runningTime;
                areaState->running        = false;
            }
        }
        break;

    case CONTROL_STATE_MANUAL:
        // TODO: add more logic
        areaState->running = true;
        break;
    }

    return areaState->running;
}

static datetime_t nullTime()
{
    datetime_t dt;
    dt.format = DTFORMAT_BIN;
    dt.year   = 0;
    dt.month  = 1;
    dt.day    = 1;
    dt.hour   = 0;
    dt.minute = 0;
    dt.second = 0;
    return dt;
}

void control_initialize(void)
{
    memset(areaStates, 0, sizeof(areaStates));
    lastWindTime     = nullTime();
    lastRainTime     = nullTime();
    currentlyRaining = false;
    currentlyWindy   = false;
}

void control_executeThread(void)
{
    int  i;
    bool allOff;
    bool areaOutput;

    if(currentlyRaining) {
        lastRainTime = datetime_now();
    }

    if(currentlyWindy) {
        lastWindTime = datetime_now();
    }

    allOff = true;
    for(i = 0; i < NUM_CONTROL_AREAS; i++) {
        areaOutput = execArea(&areaStates[i]);
        io_setOutput(areaStates[i].config.output, areaOutput);
        if(areaOutput) {
            allOff = false;
        }
    }

    io_setOutput(OUTPUT_SHUTDOWN, !allOff);
}

void control_configArea(control_area_t area, const control_config_t * config)
{
    if(area >= NUM_CONTROL_AREAS) {
        return;
    }

    areaStates[area].config = *config;
}

void control_setOutput(control_area_t area, output_t output)
{
    if(area >= NUM_CONTROL_AREAS) {
        return;
    }

    areaStates[area].config.output = output;
}

void control_setStartTime(control_area_t area, const datetime_t * starttime)
{
    if(area >= NUM_CONTROL_AREAS) {
        return;
    }

    areaStates[area].config.starttime = *starttime;
}

void control_setStopTime(control_area_t area, const datetime_t * stoptime)
{
    if(area >= NUM_CONTROL_AREAS) {
        return;
    }

    areaStates[area].config.endtime = *stoptime;
}

void control_setDuration(control_area_t area, uint16_t minutes)
{
    if(area >= NUM_CONTROL_AREAS) {
        return;
    }

    areaStates[area].config.duration_minutes = minutes;
}

void control_setWindlessPeriod(control_area_t area, uint16_t hours)
{
    if(area >= NUM_CONTROL_AREAS) {
        return;
    }

    areaStates[area].config.windless_hours = hours;
}

void control_setRainlessPeriod(control_area_t area, uint16_t hours)
{
    if(area >= NUM_CONTROL_AREAS) {
        return;
    }

    areaStates[area].config.rainless_hours = hours;
}

void control_setAreaInputState(control_area_t area, control_state_t state)
{
    if(area >= NUM_CONTROL_AREAS) {
        return;
    }

    areaStates[area].inputState = state;
}

void control_setIsRaining(bool raining)
{
    if(!raining && currentlyRaining) {
        lastRainTime = datetime_now();
    }
    currentlyRaining = raining;
}

void control_setIsWindy(bool windy)
{
    if(!windy && currentlyWindy) {
        lastWindTime = datetime_now();
    }
    currentlyWindy = windy;
}
