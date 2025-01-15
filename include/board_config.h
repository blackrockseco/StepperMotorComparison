#pragma once

#if defined(ARDUINO_AVR_UNO)
    #ifndef SERIAL_BAUD
    #define SERIAL_BAUD 115200
    #endif
    #define BOARD_NAME "Arduino Uno"
#elif defined(ESP32)
    #ifndef SERIAL_BAUD
    #define SERIAL_BAUD 115200
    #endif
    #define BOARD_NAME "ESP32"
#else
    #error "Unsupported board selected"
#endif

// Debug configuration
#define DEBUG_ENABLED
#ifdef DEBUG_ENABLED
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif