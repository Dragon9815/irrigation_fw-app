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
    control_state_t  prevInputState;

    bool       running;
    datetime_t runningSince;
    uint32_t   runningMinutes;
    datetime_t lastRunning;
    bool       isHalfAuto;

    struct
    {
        bool       running;
        datetime_t runningSince;
    } manual;
} control_areastate_t;

typedef enum
{
    RAINBARREL_LEVEL_EMPTY   = 0,
    RAINBARREL_LEVEL_BETWEEN = 1,
    RAINBARREL_LEVEL_FULL    = 2,
} rainbarrelWaterLevel_t;

typedef struct
{
    bool                   running;
    datetime_t             runningSince;
    rainbarrelWaterLevel_t prevLevel;
} rainbarrelState_t;

#define NUM_CONTROL_AREAS 3

static control_areastate_t areaStates[NUM_CONTROL_AREAS];
static datetime_t          lastRainTime;
static datetime_t          lastWindTime;
static bool                currentlyRaining;
static bool                currentlyWindy;
static bool                lowWater;
static rainbarrelState_t   rainbarrelState;
static datetime_t          lawnmowerStartTime;
static datetime_t          lawnmowerStopTime;

static void checkLastRunning(control_areastate_t * areaState, const datetime_t now)
{
    if(areaState->lastRunning.day != now.day || areaState->lastRunning.month != now.month ||
       areaState->lastRunning.year != now.year) {
        areaState->runningMinutes = 0;
    }
}

static rainbarrelWaterLevel_t getRainbarrelLevel(void)
{
    rainbarrelWaterLevel_t waterLevel;

    bool empty = !io_getInput(INPUT_RAINBARREL_EMPTY);
    bool full  = !io_getInput(INPUT_RAINBARREL_FULL);

    if(!empty && !full) {
        return RAINBARREL_LEVEL_BETWEEN;
    }
    else if(empty && !full) {
        return RAINBARREL_LEVEL_EMPTY;
    }
    else {
        return RAINBARREL_LEVEL_FULL;
    }
}

static bool processRainBarrel(rainbarrelState_t * state)
{
    if(lowWater) {
        return false;
    }
    else {
        rainbarrelWaterLevel_t waterLevel = getRainbarrelLevel();

        if(state->prevLevel != RAINBARREL_LEVEL_EMPTY && waterLevel == RAINBARREL_LEVEL_EMPTY) {
            state->running      = true;
            state->runningSince = datetime_now();
        }
        else if(waterLevel == RAINBARREL_LEVEL_FULL) {
            state->running = false;
        }

        datetime_t now = datetime_now();
        if(datetime_diffMinutes(&state->runningSince, &now) >= 120) {
            state->running = false;
        }

        state->prevLevel = waterLevel;
        return state->running;
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
        if(areaState->prevInputState == CONTROL_STATE_MANUAL) {
            areaState->isHalfAuto = true;
        }

        if(lowWater) {
            areaState->running = false;
        }
        else if(areaState->isHalfAuto) {
            if(areaState->prevInputState == CONTROL_STATE_MANUAL) {
                areaState->runningSince = datetime_now();
                areaState->running      = true;
            }
            else {
                uint32_t diff = datetime_diffMinutes(&(areaState->runningSince), &(datetime_t[]){ datetime_now() });
                areaState->runningMinutes += diff;
                if(diff >= areaState->config.manual_duration_minutes) {
                    areaState->running    = false;
                    areaState->isHalfAuto = false;
                }
            }
        }
        else {
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
                uint32_t runningTime = areaState->runningMinutes + datetime_diffMinutes(&now, &areaState->runningSince);
                if(minutesToday < startMinute || minutesToday >= endMinute ||
                   runningTime > areaState->config.duration_minutes || currentlyRaining || currentlyWindy) {
                    areaState->runningMinutes = runningTime;
                    areaState->running        = false;
                }
            }
        }
        break;

    case CONTROL_STATE_MANUAL: {
        areaState->running = false;

        //bool locked = false;

        //if(areaState->config.use_lawnmower_time) {
        //    uint32_t startMinute = datetime_getMinutesToday(&lawnmowerStartTime);
        //    uint32_t stopMinute  = datetime_getMinutesToday(&lawnmowerStopTime);
        //    if(startMinute <= minutesToday && minutesToday < stopMinute) {
        //        locked = true;
        //    }
        //}
        //if(locked) {
        //    areaState->running = false;
        //}
        //else {
        //    if(areaState->config.manual_duration_minutes > 0) {
        //        if(areaState->prevInputState != CONTROL_STATE_MANUAL) {
        //            areaState->manual.running      = true;
        //            areaState->manual.runningSince = now;
        //        }
        //        else {
        //            uint32_t deltaMinutes = datetime_diffMinutes(&areaState->manual.runningSince, &now);
        //            if(deltaMinutes >= areaState->config.manual_duration_minutes) {
        //                areaState->manual.running = false;
        //            }
        //        }
        //        areaState->running = areaState->manual.running;
        //    }
        //    else {
        //        areaState->running = true;
        //    }
        //}
    } break;
    }

    areaState->prevInputState = areaState->inputState;
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
        io_setOutput(areaStates[i].config.statusOutput, areaOutput);
        if(areaStates[i].inputState != CONTROL_STATE_OFF) {
            allOff = false;
        }
        // if(areaOutput) {
        //     allOff = false;
        // }
    }

    bool rainbarrelOutput = processRainBarrel(&rainbarrelState);
    io_setOutput(OUTPUT_RELAY_4, rainbarrelOutput);
    // if(rainbarrelOutput) {
    //     allOff = false;
    // }

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

void control_setLawnmowerStartTime(datetime_t starttime)
{
    lawnmowerStartTime = starttime;
}

void control_setLawnmowerStopTime(datetime_t stoptime)
{
    lawnmowerStopTime = stoptime;
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

void control_setLowWater(bool hasLowWater)
{
    lowWater = hasLowWater;
}