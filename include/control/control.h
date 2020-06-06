/*****************************************************************************
 * @file        control.h
 * @brief       Valve output processing and controlling
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "Common.h"
#include "io/io.h"
#include "utils/datetime.h"

typedef enum
{
    CONTROL_AREA_LAWN,
    CONTROL_AREA_HEDGE,
    CONTROL_AREA_PLANTS,
} control_area_t;

typedef enum
{
    CONTROL_STATE_OFF,
    CONTROL_STATE_AUTO,
    CONTROL_STATE_MANUAL
} control_state_t;

typedef struct
{
    output_t   output;
    datetime_t starttime;
    datetime_t endtime;
    uint16_t   duration_minutes;
    uint8_t    windless_hours;
    uint8_t    rainless_hours;
} control_config_t;

extern void control_initialize(void);
extern void control_executeThread(void);

extern void control_configArea(control_area_t area, const control_config_t * config);

extern void control_setOutput(control_area_t area, output_t output);
extern void control_setStartTime(control_area_t area, const datetime_t * starttime);
extern void control_setStopTime(control_area_t area, const datetime_t * stoptime);
extern void control_setDuration(control_area_t area, uint16_t minutes);
extern void control_setWindlessPeriod(control_area_t area, uint16_t hours);
extern void control_setRainlessPeriod(control_area_t area, uint16_t hours);

extern void control_setAreaInputState(control_area_t area, control_state_t state);
extern void control_setIsRaining(bool raining);
extern void control_setIsWindy(bool windy);

#endif /* __CONTROL_H__ */
