/*****************************************************************************
 * @file        io.h
 * @brief       Digital I/O
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#ifndef __IO_H__
#define __IO_H__

#include "Common.h"

typedef enum
{
    OUTPUT_RELAY_1 = 0,
    OUTPUT_RELAY_2,
    OUTPUT_RELAY_3,
    OUTPUT_RELAY_4,
    OUTPUT_STATUS1,
    OUTPUT_STATUS2,
    OUTPUT_STATUS3,
    OUTPUT_CISTERN_QUATER,
    OUTPUT_CISTERN_HALF,
    OUTPUT_SHUTDOWN,

    NUM_OUTPUTS
} output_t;

typedef enum
{
    INPUT_AUTO1 = 0,
    INPUT_AUTO2,
    INPUT_AUTO3,
    INPUT_MANUAL1,
    INPUT_MANUAL2,
    INPUT_MANUAL3,
    INPUT_RAIN_SENSOR,
    INPUT_WIND_SENSOR,
    INPUT_EXTRA1,
    INPUT_EXTRA2,
    INPUT_CISTERN_EMPTY,
    INPUT_CISTERN_QUATER,
    INPUT_CISTERN_HALF,
    INPUT_CISTERN_FULL,
    INPUT_RAINBARREL_EMPTY,
    INPUT_RAINBARREL_FULL,

    NUM_INPUTS
} input_t;

__SG_BEGIN_DECLS

extern void io_initialize(void);
extern void io_execThread(void);

extern void io_setOutput(output_t output, bool state);
extern void io_setAllOutputs(uint32_t state);

extern bool io_getInput(input_t input);

__SG_END_DECLS

#endif /* __IO_H__ */
