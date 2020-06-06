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

typedef enum
{
    TRINPUT_OFF  = 0,
    TRINPUT_HAND = 1,
    TRINPUT_AUTO = 2
} trinput_t;

control_state_t readTrinput(input_t inputAuto, input_t inputManual)
{
    bool bHand = !io_getInput(inputManual);
    bool bAuto = !io_getInput(inputAuto);

    if(bHand && !bAuto) {
        return CONTROL_STATE_MANUAL;
    }
    else if(!bHand && bAuto) {
        return CONTROL_STATE_AUTO;
    }
    else {
        return CONTROL_STATE_OFF;
    }
}

void printTrinputState(const char * name, trinput_t trinput)
{
    serialDebug_sendString(name);
    serialDebug_sendString(": ");
    switch(trinput) {
    case TRINPUT_OFF:
        serialDebug_sendString("OFF");
        break;

    case TRINPUT_AUTO:
        serialDebug_sendString("AUTO");
        break;

    case TRINPUT_HAND:
        serialDebug_sendString("HAND");
        break;
    }

    serialDebug_sendString("\r\n");
}

#define NUM_OUTPUTS 6

void processLine(const char * line, u32 len)
{
    if(len <= 1) {
        return;
    }

    if(strcmp(line, "help") == 0) {
        serialDebug_sendString("commands: help\n\r");
    }
    else {
        char buf[300];
        int  written = snprintf(buf, 300, "Unknown command: %s\n\r", line);
        serialDebug_sendBytes((u8 *)buf, written);
    }
}

int main(void)
{
    System_Initialize();

    halGpio_InitPort(GPIOA, gpioPortA);
    halGpio_InitPort(GPIOB, gpioPortB);
    halGpio_InitPort(GPIOC, gpioPortC);
    halGpio_InitPort(GPIOD, gpioPortD);

    io_initialize();
    serialDebug_initialize();

    control_initialize();

    Main_T0 = Timing_GetTicks_ms();
    while(true) {
        control_setAreaInputState(CONTROL_AREA_LAWN, readTrinput(INPUT_AUTO1, INPUT_MANUAL1));
        control_setAreaInputState(CONTROL_AREA_HEDGE, readTrinput(INPUT_AUTO2, INPUT_MANUAL2));
        control_setAreaInputState(CONTROL_AREA_PLANTS, readTrinput(INPUT_AUTO3, INPUT_MANUAL3));

        control_executeThread();

        io_execThread();

        Main_T1     = Timing_GetTicks_ms();
        Main_DeltaT = Main_T1 - Main_T0;
        Main_T0     = Main_T1;
        if(Main_DeltaT > Main_MaxCycleTime) {
            Main_MaxCycleTime = Main_DeltaT;
        }
    }

    return 0;
}
