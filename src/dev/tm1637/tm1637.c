#include "dev/tm1637/tm1637.h"

#include "hal/gpio/halGpio.h"

#define TM1637_I2C_COMM1 0x40
#define TM1637_I2C_COMM2 0xC0
#define TM1637_I2C_COMM3 0x80

void tm1637_initialize(void)
{}

static void setCLK(bool state)
{
    if(state) {
        halGpio_SetPin(GPIOB, HAL_GPIO_PIN_6);
    }
    else {
        halGpio_ResetPin(GPIOB, HAL_GPIO_PIN_6);
    }
}

static void setDIO(bool state)
{
    if(state) {
        halGpio_SetPin(GPIOB, HAL_GPIO_PIN_7);
    }
    else {
        halGpio_ResetPin(GPIOB, HAL_GPIO_PIN_7);
    }
}

static void setDIOOuput()
{
    halGpio_SetMode(GPIOB, 7, HAL_GPIO_MODE_OUTPUT);
}

static void setDIOInput()
{
    halGpio_SetMode(GPIOB, 7, HAL_GPIO_MODE_INPUT);
}

static bool readDIO()
{
    return halGpio_GetPin(GPIOB, HAL_GPIO_PIN_7);
}

static void display_bitDelay(void)
{
    for(int i = 0; i < 100; i++) {
        __NOP();
    }
}

static void display_start(void)
{
    setCLK(true);
    display_bitDelay();
    setDIO(true);
    display_bitDelay();
    setDIO(false);
    display_bitDelay();
    setCLK(false);
    display_bitDelay();
}

static void display_stop(void)
{
    setCLK(false);
    display_bitDelay();
    setDIO(false);
    display_bitDelay();
    setCLK(true);
    display_bitDelay();
    setDIO(true);
    display_bitDelay();
}

static bool display_writeByte(uint8_t b)
{
    uint8_t data = b;

    for(uint8_t i = 0; i < 8; i++) {
        setCLK(false);
        display_bitDelay();
        setDIO((data & 1) != 0);
        display_bitDelay();
        setCLK(true);
        display_bitDelay();
        data >>= 1;
    }

    setCLK(false);
    display_bitDelay();
    setDIO(true);
    display_bitDelay();
    setCLK(true);
    setDIOInput();

    display_bitDelay();
    bool ack = readDIO();

    if(ack) {
        setDIOOuput();
        setDIO(false);
    }

    display_bitDelay();
    setDIOOuput();
    display_bitDelay();

    return ack;
}

void tm1637_sendDisplayData(const uint8_t displayBuffer[static 4], bool dotEnabled)
{
    display_start();
    display_writeByte(TM1637_I2C_COMM1);
    display_stop();

    // Write COMM2 + first digit address
    display_start();
    display_writeByte(TM1637_I2C_COMM2);

    // Write the data bytes
    display_writeByte(displayBuffer[0]);
    if(dotEnabled) {
        display_writeByte(displayBuffer[1] | 0x80);
    }
    else {
        display_writeByte(displayBuffer[1]);
    }
    display_writeByte(displayBuffer[2]);
    display_writeByte(displayBuffer[3]);

    display_stop();

    // Write COMM3 + brightness
    display_start();
    display_writeByte(TM1637_I2C_COMM3 | 0x0f);
    display_stop();
}