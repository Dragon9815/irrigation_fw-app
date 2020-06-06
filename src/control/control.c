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
} control_areastate_t;

#define NUM_CONTROL_AREAS 3

static control_areastate_t areaStates[NUM_CONTROL_AREAS];
static datetime_t          lastRainTime;
static datetime_t          lastWindTime;
static bool                currentlyRaining;
static bool                currentlyWindy;

static bool execArea(control_areastate_t * areaState)
{
    datetime_t now;
    uint32_t   nowMinutes;
    uint32_t   startMinute;
    uint32_t   endMinute;

    datetime_now(&now);
    nowMinutes = datetime_getMinutesToday(&now);

    switch(areaState->inputState) {
    case CONTROL_STATE_OFF:
        areaState->running = false;
        break;

    case CONTROL_STATE_AUTO:
        startMinute = datetime_getMinutesToday(&areaState->config.starttime);
        endMinute   = datetime_getMinutesToday(&areaState->config.endtime);

        if(!areaState->running) {
            if(nowMinutes >= startMinute && nowMinutes < endMinute &&
               datetime_diffMinutes(&now, &areaState->runningSince) < areaState->config.duration_minutes &&
               datetime_diffHours(&now, &lastRainTime) >= areaState->config.rainless_hours &&
               datetime_diffHours(&now, &lastWindTime) >= areaState->config.windless_hours) {
                areaState->running      = true;
                areaState->runningSince = now;
            }
        }
        else {
            if(nowMinutes < startMinute || nowMinutes <= endMinute ||
               datetime_diffMinutes(&now, &areaState->runningSince) > areaState->config.duration_minutes ||
               currentlyRaining || currentlyWindy) {
                areaState->running = false;
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

void control_initialize(void)
{
    memset(areaStates, 0, sizeof(areaStates));
}

void control_executeThread(void)
{
    int  i;
    bool allOff;
    bool areaOutput;

    if(currentlyRaining) {
        datetime_now(&lastRainTime);
    }

    if(currentlyWindy) {
        datetime_now(&lastWindTime);
    }

    allOff = true;
    for(i = 0; i < NUM_CONTROL_AREAS; i++) {
        areaOutput = execArea(&areaStates[i]);
        io_setOutput(areaStates[i].config.output, areaOutput);
        if(areaOutput) {
            allOff = false;
        }
    }

    //    setRelayOuput(3, !allOff);
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
    currentlyRaining = raining;
}

void control_setIsWindy(bool windy)
{
    currentlyWindy = windy;
}
