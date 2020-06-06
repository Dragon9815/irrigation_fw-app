cmake_minimum_required(VERSION 3.12)

enable_language(ASM)

set(STM32_TARGET_DEVICE STM32F446xx)
set(STM32_HSE_VALUE 8000000)
set(STM32HAL_NEEDED_COMPONENTS adc can spi tim uart gpio dma uart rcc flash pwr cortex)
set(STM32HAL_USE_LL true)
set(STM32HAL_NEEDED_LL_COMPONENTS dma spi gpio usart rcc )

add_subdirectory(vendor/stm32f4-cmsis)
add_subdirectory(vendor/stm32f4-hal)

set(PROJECT_NAME irrigation_mcuapp)

project(${PROJECT_NAME})

set(sources
    config/gpioDefs.c
    control/control.c
    dev/eeprom/EEPROM_LowLevel.c
    dev/eeprom/EEPROM.c
    dev/ethernet/w5500_lowlevel.c
    dev/spi/SPI_Common.c
    dev/spi/SPI_EEPROM.c
    dev/spi/SPI_WIZ.c
    dev/timer_common/Timer_Common.c
    dev/uart/UART_Common.c
    dev/uart/UART_Config.c
    hal/gpio/halGpio.c
    io/io.c
    protocol/socket.c
    security/SHA512.c
    serial/serialDebug.c
    system/newlib-syscalls.c
    system/startup_stm32f446xx.s
    system/syscall.c
    system/System.c
    system/SystemISR.c
    system/tasking.c
    system/timing.c
    utils/CircularQueue.c
    utils/crc16.c
    utils/CRC32C.c
    utils/datetime.c
    utils/Utils.c
    Debug.c
    Error.c
    main.c
    )

list(TRANSFORM sources PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_executable(${PROJECT_NAME} ${sources})
target_link_options(${PROJECT_NAME} PRIVATE "-T${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.ld")
target_compile_definitions(${PROJECT_NAME} PRIVATE "HW_VERSION=1")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} stm32f4-cmsis stm32f4-hal)
