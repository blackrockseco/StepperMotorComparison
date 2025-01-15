// include/motor_config.h
#pragma once
#include "board_config.h"
#include "pin_config.h"  // Include the separate pin configuration file for clarity

// Default Motion Parameters
constexpr float DEFAULT_MAX_SPEED_NEMA = 2000.0;  // steps/second
constexpr float DEFAULT_ACCELERATION_NEMA = 1000.0;  // steps/second²
constexpr float DEFAULT_MAX_SPEED_BYJ = 500.0;     // steps/second
constexpr float DEFAULT_ACCELERATION_BYJ = 200.0;  // steps/second²

// TMC2209 Driver Pin Configuration
struct TMC2209PinConfig {
    const int stepPin;
    const int dirPin;
    const int enablePin;
    const int ms1Pin;
    const int ms2Pin;

    // Constructor using pin assignments from pin_config.h
    constexpr TMC2209PinConfig(int step = TMC2209_STEP_PIN, int dir = TMC2209_DIR_PIN, int enable = TMC2209_ENABLE_PIN, int ms1 = TMC2209_MS1_PIN, int ms2 = TMC2209_MS2_PIN)
        : stepPin(step)
        , dirPin(dir)
        , enablePin(enable)
        , ms1Pin(ms1)
        , ms2Pin(ms2) {}
};

// ULN2003 Driver Pin Configuration
struct ULN2003PinConfig {
    const int in1Pin;  // Input 1 for motor coil 1
    const int in2Pin;  // Input 2 for motor coil 1
    const int in3Pin;  // Input 1 for motor coil 2
    const int in4Pin;  // Input 2 for motor coil 2

    // Constructor using pin assignments from pin_config.h
    constexpr ULN2003PinConfig(int in1 = ULN2003_IN1_PIN, int in2 = ULN2003_IN2_PIN, int in3 = ULN2003_IN3_PIN, int in4 = ULN2003_IN4_PIN)
        : in1Pin(in1)
        , in2Pin(in2)
        , in3Pin(in3)
        , in4Pin(in4) {}
};

// Factory Function for Pin Assignments Based on Board Type
inline TMC2209PinConfig getTMC2209PinConfig() {
#if defined(ARDUINO_AVR_UNO)
    return TMC2209PinConfig();  // Uses default pins defined in pin_config.h for Arduino Uno
#elif defined(ESP32)
    return TMC2209PinConfig();  // Uses default pins defined in pin_config.h for ESP32
#else
    #error "Unsupported board type"
#endif
}

inline ULN2003PinConfig getULN2003PinConfig() {
#if defined(ARDUINO_AVR_UNO)
    return ULN2003PinConfig();  // Uses default pins defined in pin_config.h for Arduino Uno
#elif defined(ESP32)
    return ULN2003PinConfig();  // Uses default pins defined in pin_config.h for ESP32
#else
    #error "Unsupported board type"
#endif
}

// The `pin_config.h` file should define constants like TMC2209_STEP_PIN, ULN2003_IN1_PIN, etc.
// This keeps all pin assignments centralized and ensures maintainability.
