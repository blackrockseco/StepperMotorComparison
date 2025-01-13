// include/motor_config.h
#pragma once
#include "board_config.h"

// Pin configuration for TMC2209 driver
struct TMC2209PinConfig {
    const int stepPin;
    const int dirPin;
    const int enablePin;
    const int ms1Pin;
    const int ms2Pin;

    TMC2209PinConfig(int step, int dir, int enable, int ms1, int ms2)
        : stepPin(step)
        , dirPin(dir)
        , enablePin(enable)
        , ms1Pin(ms1)
        , ms2Pin(ms2) {}
};

// Pin configuration for ULN2003 driver
struct ULN2003PinConfig {
    const int in1Pin;  // Step
    const int in2Pin;  // Dir
    const int in3Pin;
    const int in4Pin;

    ULN2003PinConfig(int in1, int in2, int in3, int in4)
        : in1Pin(in1)
        , in2Pin(in2)
        , in3Pin(in3)
        , in4Pin(in4) {}
};