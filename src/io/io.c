/*****************************************************************************
 * @file        io.c
 * @brief       Digital I/O
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "io/io.h"

#include "hal/gpio/halGpio.h"
#include "system/timing.h"

#include <string.h>

#define SECS    *1000
#define MINUTES *60 SECS

typedef struct
{
    GPIO_TypeDef * port;
    uint32_t       pin;
    bool           inverted;
    bool           filtered;
    uint32_t       filterTime;
    uint32_t       longPressTime;
} iodef_t;

// clang-format off
const iodef_t inputs[NUM_INPUTS] = {
    /*                                                        filtered,         */
    /* Name                      Port , Pin,           inverted,     filterTime */
    [INPUT_AUTO1]            = { GPIOC, HAL_GPIO_PIN_0,  true,  false },
    [INPUT_AUTO2]            = { GPIOC, HAL_GPIO_PIN_1,  true,  false },
    [INPUT_AUTO3]            = { GPIOC, HAL_GPIO_PIN_2,  true,  false },
    [INPUT_MANUAL1]          = { GPIOC, HAL_GPIO_PIN_3,  true,  false },
    [INPUT_MANUAL2]          = { GPIOC, HAL_GPIO_PIN_4,  true,  false },
    [INPUT_MANUAL3]          = { GPIOC, HAL_GPIO_PIN_5,  true,  false },
    [INPUT_RAIN_SENSOR]      = { GPIOC, HAL_GPIO_PIN_6,  true,  true,  15 MINUTES },
    [INPUT_WIND_SENSOR]      = { GPIOC, HAL_GPIO_PIN_7,  true,  true,  15 MINUTES },
    [INPUT_EXTRA1]           = { GPIOC, HAL_GPIO_PIN_8,  true,  false },
    [INPUT_EXTRA2]           = { GPIOC, HAL_GPIO_PIN_9,  true,  false },
    [INPUT_CISTERN_EMPTY]    = { GPIOA, HAL_GPIO_PIN_8,  true,  true,  15 SECS },
    [INPUT_CISTERN_QUATER]   = { GPIOA, HAL_GPIO_PIN_9,  true,  true,  15 SECS },
    [INPUT_CISTERN_HALF]     = { GPIOA, HAL_GPIO_PIN_10, true,  true,  15 SECS },
    [INPUT_CISTERN_FULL]     = { GPIOA, HAL_GPIO_PIN_11, true,  true,  15 SECS },
    [INPUT_RAINBARREL_EMPTY] = { GPIOA, HAL_GPIO_PIN_0,  true,  true,  15 SECS },
    [INPUT_RAINBARREL_FULL]  = { GPIOA, HAL_GPIO_PIN_1,  true,  true,  15 SECS },
    [INPUT_CLOCK_SET]        = { GPIOB, HAL_GPIO_PIN_0,  false, false, 0,      5 SECS },
    [INPUT_CLOCK_PLUS]       = { GPIOB, HAL_GPIO_PIN_4,  false, false },
    [INPUT_CLOCK_MINUS]      = { GPIOB, HAL_GPIO_PIN_5,  false, false },
};
// clang-format on

typedef struct
{
    bool     stateRaw;
    uint32_t filterStartTime;
    bool     stateFiltered;
    uint32_t pressStartTime;
    bool     stateLongPressed;
    bool     edgePos;
    bool     edgeNeg;
} inputState_t;

static inputState_t inputStates[NUM_INPUTS];
static uint32_t     outputState;

void io_initialize(void)
{
    outputState = 0;
    memset(inputStates, 0, sizeof(inputStates));
}

static void setData(bool state)
{
    if(state) {
        halGpio_SetPin(GPIOB, HAL_GPIO_PIN_8);
    }
    else {
        halGpio_ResetPin(GPIOB, HAL_GPIO_PIN_8);
    }
}

static void setClock(bool state)
{
    if(state) {
        halGpio_SetPin(GPIOB, HAL_GPIO_PIN_9);
    }
    else {
        halGpio_ResetPin(GPIOB, HAL_GPIO_PIN_9);
    }
}

static void setLatch(bool state)
{
    if(state) {
        halGpio_SetPin(GPIOB, HAL_GPIO_PIN_10);
    }
    else {
        halGpio_ResetPin(GPIOB, HAL_GPIO_PIN_10);
    }
}

static void delay_us(uint32_t delay)
{
    uint32_t delay_counter = 0;
    delay *= 16; // estimate 1 us
    while(delay_counter < delay) {
        delay_counter++;
    }
}

static bool pinFilter(const inputState_t pinState, const iodef_t config)
{
    bool resultState = pinState.stateFiltered;

    if(config.filtered) {
        if(pinState.stateFiltered != pinState.stateRaw) {
            uint32_t deltaT = Timing_GetTicks_ms() - pinState.filterStartTime;
            if(deltaT >= config.filterTime) {
                resultState = pinState.stateRaw;
            }
        }
    }
    else {
        resultState = pinState.stateRaw;
    }

    return resultState;
}

static bool filterLongPress(const inputState_t pinState, const iodef_t config)
{
    bool resultState = pinState.stateLongPressed;

    if(pinState.stateLongPressed != pinState.stateFiltered) {
        if(pinState.stateFiltered == false) {
            resultState = false;
        }
        else {
            uint32_t deltaT = Timing_GetTicks_ms() - pinState.pressStartTime;
            if(deltaT >= config.longPressTime) {
                resultState = pinState.stateFiltered;
            }
        }
    }

    return resultState;
}

static void processPin(int pinIdx, bool newState)
{
    if(inputs[pinIdx].inverted) {
        newState = !newState;
    }

    inputStates[pinIdx].edgePos = 0;
    inputStates[pinIdx].edgeNeg = 0;

    if(newState != inputStates[pinIdx].stateRaw) {
        inputStates[pinIdx].filterStartTime = Timing_GetTicks_ms();
        inputStates[pinIdx].stateRaw        = newState;
    }

    bool newStateFiltered = pinFilter(inputStates[pinIdx], inputs[pinIdx]);
    if(newStateFiltered != inputStates[pinIdx].stateFiltered) {
        if(newStateFiltered) {
            inputStates[pinIdx].edgePos = true;
        }
        else {
            inputStates[pinIdx].edgeNeg = true;
        }
        inputStates[pinIdx].stateFiltered  = newStateFiltered;
        inputStates[pinIdx].pressStartTime = Timing_GetTicks_ms();
    }

    inputStates[pinIdx].stateLongPressed = filterLongPress(inputStates[pinIdx], inputs[pinIdx]);
}

void io_execThread(void)
{
    int      i;
    bool     newState;
    uint32_t deltaT;

    for(i = 0; i < NUM_OUTPUTS; i++) {
        setData((outputState >> (NUM_OUTPUTS - i - 1)) & 1);
        delay_us(10);
        setClock(true);
        delay_us(10);
        setClock(false);
        delay_us(10);
    }

    setLatch(true);
    delay_us(10);
    setLatch(false);
    delay_us(10);

    for(i = 0; i < NUM_INPUTS; i++) {
        newState = halGpio_GetPin(inputs[i].port, inputs[i].pin);

        processPin(i, newState);
    }
}

void io_setOutput(output_t output, bool state)
{
    if(output >= 0 && output < NUM_OUTPUTS) {
        if(state) {
            outputState |= (1 << output);
        }
        else {
            outputState &= ~(1 << output);
        }
    }
}

void io_setAllOutputs(uint32_t state)
{
    outputState = state;
}

bool io_getInput(input_t input)
{
    if(input < 0 || input >= NUM_INPUTS) {
        return false;
    }

    return inputStates[input].stateFiltered;
}

bool io_getInputLongPress(input_t input)
{
    if(input < 0 || input >= NUM_INPUTS) {
        return false;
    }

    return inputStates[input].stateLongPressed;
}

bool io_getInputEdgePos(input_t input)
{
    if(input < 0 || input >= NUM_INPUTS) {
        return false;
    }

    return inputStates[input].edgePos;
}

bool io_getInputEdgeNeg(input_t input)
{
    if(input < 0 || input >= NUM_INPUTS) {
        return false;
    }

    return inputStates[input].edgeNeg;
}
