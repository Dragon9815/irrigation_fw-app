/***************************************************************************** 
 * @file        main.c
 * @brief       Entry point for the MCU
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */

#include "Ident.h"
#include "Version.h"
#include "config/gpioDefs.h"
#include "control/control.h"
#include "dev/eeprom/EEPROM.h"
#include "dev/eeprom/EEPROM_LowLevel.h"
#include "dev/rtc/rtc.h"
#include "dev/rtc/rtcDisplay.h"
#include "dev/spi/SPI_WIZ.h"
#include "dev/uart/UART_Config.h"
#include "hal/gpio/halGpio.h"
#include "io/io.h"
#include "mcu.h"
#include "serial/serialDebug.h"
#include "system/System.h"
#include "system/tasking.h"
#include "system/timing.h"
#include "utils/memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Ident_t Ident = { 0 };

u32 Main_T0, Main_T1, Main_DeltaT;
u32 Main_MaxCycleTime;

void Ident_Initalize()
{
    Ident.FWVersion.Major    = FW_VERSION_MAJOR;
    Ident.FWVersion.Minor    = FW_VERSION_MINOR;
    Ident.FWVersion.Subminor = FW_VERSION_SUBMINOR;

    Ident.BLVersion.Major    = BL_VERSION_MAJOR;
    Ident.BLVersion.Minor    = BL_VERSION_MINOR;
    Ident.BLVersion.Subminor = BL_VERSION_SUBMINOR;

    Ident.HWVersion = HW_VERSION;

    Ident.SerialNumber   = 0;
    Ident.Revision       = 1;
    Ident.AdditionalInfo = 0;
    Ident.ProductCode    = 0;

    Ident.MCU_UniqueId_Word0 = Memory_ReadWord32(0x1FFF7A10 + 0);
    Ident.MCU_UniqueId_Word1 = Memory_ReadWord32(0x1FFF7A10 + 4);
    Ident.MCU_UniqueId_Word2 = Memory_ReadWord32(0x1FFF7A10 + 8);

    Ident.MCU_ChipTypeId = Memory_ReadWord32(0xE0042000);
    Ident.MCU_FlashSize  = Memory_ReadWord16(0x1FFF7A22);
}

#define NUM_TRINPUTS        3
#define TRINPUT_FILTER_TIME 3000

typedef enum
{
    TRINPUT_SPRINKLER = 0,
    TRINPUT_HEDGE,
    TRINPUT_PLANTS
} trinput_t;

typedef struct
{
    control_state_t rawState;
    control_state_t filteredState;
    uint32_t        filterT0;

    struct
    {
        input_t inputAuto;
        input_t inputManual;
    } config;
} trinput_state_t;

trinput_state_t trinputs[NUM_TRINPUTS] = {
    [TRINPUT_SPRINKLER] = { .config = { INPUT_AUTO1, INPUT_MANUAL1 } },
    [TRINPUT_HEDGE]     = { .config = { INPUT_AUTO2, INPUT_MANUAL2 } },
    [TRINPUT_PLANTS]    = { .config = { INPUT_AUTO3, INPUT_MANUAL3 } },
};

control_state_t readRawTrinputState(trinput_t trinput)
{
    if(trinput >= 0 && trinput < NUM_TRINPUTS) {
        bool bHand = io_getInput(trinputs[trinput].config.inputManual);
        bool bAuto = io_getInput(trinputs[trinput].config.inputAuto);

        if(bHand && bAuto) {
            return CONTROL_STATE_MANUAL;
        }
        else if(!bHand && bAuto) {
            return CONTROL_STATE_AUTO;
        }
        else {
            return CONTROL_STATE_OFF;
        }
    }
    else {
        return CONTROL_STATE_OFF;
    }
}

control_state_t readTrinput(trinput_t trinput)
{
    if(trinput >= 0 && trinput < NUM_TRINPUTS) {
        control_state_t newState = readRawTrinputState(trinput);

        if(newState != trinputs[trinput].rawState) {
            trinputs[trinput].rawState = newState;
            trinputs[trinput].filterT0 = Timing_GetTicks_ms();
        }

        if(trinputs[trinput].rawState != trinputs[trinput].filteredState) {
            uint32_t deltaT = Timing_GetTicks_ms() - trinputs[trinput].filterT0;
            if(deltaT >= TRINPUT_FILTER_TIME) {
                trinputs[trinput].filteredState = trinputs[trinput].rawState;
            }
        }

        return trinputs[trinput].filteredState;
    }
    else {
        return CONTROL_STATE_OFF;
    }
}

// control_state_t readTrinput(input_t inputAuto, input_t inputManual)
// {
//     bool bHand = io_getInput(inputManual);
//     bool bAuto = io_getInput(inputAuto);

//     if(bHand && !bAuto) {
//         return CONTROL_STATE_MANUAL;
//     }
//     else if(!bHand && bAuto) {
//         return CONTROL_STATE_AUTO;
//     }
//     else {
//         return CONTROL_STATE_OFF;
//     }
// }

// void printTrinputState(const char * name, trinput_t trinput)
// {
//     serialDebug_sendString(name);
//     serialDebug_sendString(": ");
//     switch(trinput) {
//     case TRINPUT_OFF:
//         serialDebug_sendString("OFF");
//         break;

//     case TRINPUT_AUTO:
//         serialDebug_sendString("AUTO");
//         break;

//     case TRINPUT_HAND:
//         serialDebug_sendString("HAND");
//         break;
//     }

//     serialDebug_sendString("\r\n");
// }

// #define NUM_OUTPUTS 6

void processLine(const char * line, u32 len)
{
    serialDebug_sendString("\n\r");

    char * token;

    token = strtok(line, " ");

    if(strncmp(token, "help", 4) == 0) {
        serialDebug_sendString("commands: help, date\n\r");
    }
    else if(strncmp(token, "date", 4) == 0) {
        char       buf[300];
        datetime_t now = datetime_now();
        now            = datetime_toFormat(&now, DTFORMAT_BIN);
        int written    = snprintf(buf, 300, "%02d.%02d.%02dT%02d:%02d:%02d\n\r", now.day, now.month, now.year, now.hour,
                               now.minute, now.second);
        serialDebug_sendBytes((u8 *)buf, written);
    }
    else if(token != NULL) {
        char buf[300];
        int  written = snprintf(buf, 300, "Unknown command: %s\n\r", token);
        serialDebug_sendBytes((u8 *)buf, written);
    }
}

#define MAX_LINE_LENGTH 256
#define HISTORY_COUNT   10
char history[HISTORY_COUNT * (MAX_LINE_LENGTH + 1)];
int  historyIndex;
u8   lineBufferTemp[MAX_LINE_LENGTH + 1]; // one extra byte for zero terminator
u8   lineBuffer[MAX_LINE_LENGTH + 1];     // one extra byte for zero terminator
u32  lineIndex;
bool escapeSeen;
bool inEscapeSequence;

void shell_init(void)
{
    historyIndex     = -1;
    lineIndex        = 0;
    escapeSeen       = false;
    inEscapeSequence = false;
    serialDebug_sendString("\n\rIrrigation v0.1.0\n\r\n\r$ ");
}

bool shell_processChar(u8 c)
{
    if(escapeSeen) {
        if(c == '[') {
            inEscapeSequence = true;
        }
        escapeSeen = false;
    }
    else if(inEscapeSequence) {
        switch(c) {
        case 'A': // up arrow
            historyIndex++;
            if(historyIndex >= HISTORY_COUNT) {
                historyIndex = HISTORY_COUNT - 1;
            }
            break;

        case 'B': // down arrow
            historyIndex--;
            if(historyIndex < -1) {
                historyIndex = -1;
            }
            break;

        case 'C': // right arrow
            break;

        case 'D': // left arrow
            break;
        }


        inEscapeSequence = false;
    }
    else {
        if(c == '\r') {
            lineBuffer[lineIndex] = 0;
            return true;
        }
        else if(c == '\b') {
            if(lineIndex > 0) {
                lineIndex--;
                serialDebug_sendString("\b \b");
            }
            return false;
        }
        else if(c == '\033') {
            escapeSeen = true;
            return false;
        }
        else {
            if(isprint(c)) {
                serialDebug_sendByte(c);
                lineBuffer[lineIndex] = c;
                lineIndex++;
                return (lineIndex >= MAX_LINE_LENGTH);
            }
            else {
                return false;
            }
        }
    }
}

void shell_execThread(void)
{
    u8 rxByte;
    if(serialDebug_readBytes(&rxByte, 1) == 1) {
        if(shell_processChar(rxByte)) {
            processLine(lineBuffer, lineIndex);
            memmove(history + (MAX_LINE_LENGTH + 1), history, (HISTORY_COUNT - 1) * (MAX_LINE_LENGTH + 1));
            memcpy(history, lineBuffer, lineIndex);
            lineIndex = 0;
            serialDebug_sendString("$ ");
        }
    }
}

typedef enum
{
    CISTERN_LEVEL_EMPTY      = 0,
    CISTERN_LEVEL_NEAR_EMPTY = 1,
    CISTERN_LEVEL_QUATER     = 2,
    CISTERN_LEVEL_HALF       = 3,
    CISTERN_LEVEL_FULL       = 4,
} cisternWaterLevel_t;

static cisternWaterLevel_t getAndDisplayWaterLevel(void)
{
    cisternWaterLevel_t waterLevel;

    if(io_getInput(INPUT_CISTERN_FULL)) {
        waterLevel = CISTERN_LEVEL_FULL;
    }
    else if(io_getInput(INPUT_CISTERN_HALF)) {
        waterLevel = CISTERN_LEVEL_HALF;
    }
    else if(io_getInput(INPUT_CISTERN_QUATER)) {
        waterLevel = CISTERN_LEVEL_QUATER;
    }
    else if(io_getInput(INPUT_CISTERN_EMPTY)) {
        waterLevel = CISTERN_LEVEL_NEAR_EMPTY;
    }
    else {
        waterLevel = CISTERN_LEVEL_EMPTY;
    }

    io_setOutput(OUTPUT_CISTERN_QUATER, waterLevel >= CISTERN_LEVEL_QUATER);
    io_setOutput(OUTPUT_CISTERN_HALF, waterLevel >= CISTERN_LEVEL_HALF);

    return waterLevel;
}

int main(void)
{
    control_config_t area_config;

    System_Initialize();

    halGpio_InitPort(GPIOA, gpioPortA);
    halGpio_InitPort(GPIOB, gpioPortB);
    halGpio_InitPort(GPIOC, gpioPortC);
    halGpio_InitPort(GPIOD, gpioPortD);

    io_initialize();
    rtc_initialize();

    serialDebug_initialize();
    shell_init();

    control_initialize();

    area_config.output                  = OUTPUT_RELAY_1;
    area_config.statusOutput            = OUTPUT_STATUS1;
    area_config.starttime               = datetime_fromTime(3, 0, 0);
    area_config.endtime                 = datetime_fromTime(5, 0, 0);
    area_config.duration_minutes        = 30;
    area_config.windless_hours          = 0;
    area_config.rainless_hours          = 24 * 2;
    area_config.manual_duration_minutes = 30;
    area_config.use_lawnmower_time      = true;
    control_configArea(CONTROL_AREA_LAWN, &area_config);

    area_config.output                  = OUTPUT_RELAY_2;
    area_config.statusOutput            = OUTPUT_STATUS2;
    area_config.starttime               = datetime_fromTime(3, 0, 0);
    area_config.endtime                 = datetime_fromTime(5, 0, 0);
    area_config.duration_minutes        = 120;
    area_config.windless_hours          = 0;
    area_config.rainless_hours          = 24 * 5;
    area_config.manual_duration_minutes = 30;
    control_configArea(CONTROL_AREA_HEDGE, &area_config);

    area_config.output                  = OUTPUT_RELAY_3;
    area_config.statusOutput            = OUTPUT_STATUS3;
    area_config.starttime               = datetime_fromTime(3, 0, 0);
    area_config.endtime                 = datetime_fromTime(7, 0, 0);
    area_config.duration_minutes        = 180;
    area_config.windless_hours          = 0;
    area_config.rainless_hours          = 24 * 2;
    area_config.manual_duration_minutes = 30;
    control_configArea(CONTROL_AREA_PLANTS, &area_config);

    control_setLawnmowerStartTime(datetime_fromTime(20, 0, 0));
    control_setLawnmowerStopTime(datetime_fromTime(22, 0, 0));

    rtcDisplay_initialize();

    Main_T0 = Timing_GetTicks_ms();
    while(true) {
        //shell_execThread();

        // datetime_t dt;
        // rtc_getCurrentDateTime(&dt);
        // datetime_t dt_bin = datetime_toFormat(&dt, DTFORMAT_BIN);
        // dt_bin.year++;
        // if(dt_bin.year > 99) {
        //     dt_bin.year = 0;
        // }
        // dt = datetime_toFormat(&dt_bin, DTFORMAT_BCD);
        // rtc_setCurrentDateTime(&dt);

        io_execThread();

        cisternWaterLevel_t waterLevel = getAndDisplayWaterLevel();

        control_setAreaInputState(CONTROL_AREA_LAWN, readTrinput(TRINPUT_SPRINKLER));
        control_setAreaInputState(CONTROL_AREA_HEDGE, readTrinput(TRINPUT_HEDGE));
        control_setAreaInputState(CONTROL_AREA_PLANTS, readTrinput(TRINPUT_PLANTS));
        control_setIsRaining(io_getInput(INPUT_RAIN_SENSOR));
        control_setIsWindy(io_getInput(INPUT_WIND_SENSOR));
        control_setLowWater(waterLevel < CISTERN_LEVEL_QUATER);
        control_executeThread();

        rtcDisplay_execThread();

        Main_T1     = Timing_GetTicks_ms();
        Main_DeltaT = Main_T1 - Main_T0;
        Main_T0     = Main_T1;
        if(Main_DeltaT > Main_MaxCycleTime) {
            Main_MaxCycleTime = Main_DeltaT;
        }
    }

    return 0;
}
