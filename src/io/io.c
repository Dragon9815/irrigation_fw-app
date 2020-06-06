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
    bool           filtered;
    uint32_t       filterTime;
} iodef_t;

const iodef_t inputs[NUM_INPUTS] = {
    { GPIOC, HAL_GPIO_PIN_0, true, 5 SECS },    { GPIOC, HAL_GPIO_PIN_1, true, 5 SECS },
    { GPIOC, HAL_GPIO_PIN_2, true, 5 SECS },    { GPIOC, HAL_GPIO_PIN_3, true, 5 SECS },
    { GPIOC, HAL_GPIO_PIN_4, true, 5 SECS },    { GPIOC, HAL_GPIO_PIN_5, true, 5 SECS },
    { GPIOC, HAL_GPIO_PIN_6, true, 5 MINUTES }, { GPIOC, HAL_GPIO_PIN_7, true, 5 MINUTES },
    { GPIOC, HAL_GPIO_PIN_8, false },           { GPIOC, HAL_GPIO_PIN_9, false },
    { GPIOA, HAL_GPIO_PIN_0, true, 15 SECS },   { GPIOA, HAL_GPIO_PIN_1, true, 15 SECS },
    { GPIOA, HAL_GPIO_PIN_8, true, 15 SECS },   { GPIOA, HAL_GPIO_PIN_9, true, 15 SECS },
    { GPIOA, HAL_GPIO_PIN_10, true, 15 SECS },  { GPIOA, HAL_GPIO_PIN_11, true, 15 SECS }
};

typedef struct
{
    bool     stateRaw;
    uint32_t filterStartTime;
    bool     stateFiltered;
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
    delay *= 180; // estimate 1 us
    while(delay_counter < delay) {
        delay_counter++;
    }
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

        if(newState != inputStates[i].stateRaw) {
            inputStates[i].filterStartTime = Timing_GetTicks_ms();
            inputStates[i].stateRaw        = newState;
        }

        if(inputStates[i].stateRaw != inputStates[i].stateFiltered) {
            deltaT = Timing_GetTicks_ms() - inputStates[i].filterStartTime;
            if(deltaT >= inputs[i].filterTime) {
                inputStates[i].stateFiltered = inputStates[i].stateRaw;
            }
        }
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
