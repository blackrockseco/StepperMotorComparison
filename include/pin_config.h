// include/pin_config.h
#pragma once

// **Pin Configuration for TMC2209 + NEMA 11 and ULN2003 + 28BYJ-48**

// TMC2209 Pin Assignments (for ESP32 and Arduino Uno)
#if defined(ARDUINO_AVR_UNO)
    #define TMC2209_STEP_PIN 2
    #define TMC2209_DIR_PIN 3
    #define TMC2209_ENABLE_PIN 4
    #define TMC2209_MS1_PIN 5
    #define TMC2209_MS2_PIN 6
#elif defined(ESP32)
    #define TMC2209_STEP_PIN 4
    #define TMC2209_DIR_PIN 2
    #define TMC2209_ENABLE_PIN 5  // Shared across platforms
    #define TMC2209_MS1_PIN 17
    #define TMC2209_MS2_PIN 16
#else
    #error "Unsupported board type for TMC2209 pins"
#endif

// ULN2003 Pin Assignments (for ESP32 and Arduino Uno)
#if defined(ARDUINO_AVR_UNO)
    #define ULN2003_IN1_PIN 10
    #define ULN2003_IN2_PIN 11
    #define ULN2003_IN3_PIN 12
    #define ULN2003_IN4_PIN 13
#elif defined(ESP32)
    #define ULN2003_IN1_PIN 33
    #define ULN2003_IN2_PIN 32
    #define ULN2003_IN3_PIN 35
    #define ULN2003_IN4_PIN 34
#else
    #error "Unsupported board type for ULN2003 pins"
#endif

// **Notes:**
// - The pin assignments for the TMC2209 and ULN2003 drivers can be adjusted if needed.
// - Ensure that the chosen pins correspond to valid GPIO pins for the selected microcontroller.
// - These constants are referenced by the `motor_config.h` file to initialize the driver structs.