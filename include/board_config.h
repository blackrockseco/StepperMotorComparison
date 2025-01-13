// board_config.h
#pragma once

#if defined(ARDUINO_AVR_UNO)
    #define SERIAL_BAUD 115200
    #define BOARD_NAME "Arduino Uno"
#elif defined(ESP32)
    #define SERIAL_BAUD 115200
    #define BOARD_NAME "ESP32"
#else
    #error "Unsupported board selected"
#endif