// include/motor_config.h
#pragma once
#include "board_config.h"

// Pin configurations per board type
struct PinConfig {
    const int stepPin;
    const int dirPin;
    const int enablePin;
    const int ms1Pin;
    const int ms2Pin;
};

// Define pins for each board and driver combination
#if defined(ARDUINO_AVR_UNO)
    const PinConfig TMC2209_PINS = {
        .stepPin = 7,
        .dirPin = 8,
        .enablePin = 4,
        .ms1Pin = 5,
        .ms2Pin = 6
    };
    
    const PinConfig ULN2003_PINS = {
        .stepPin = 8,
        .dirPin = 9,
        .enablePin = 10,
        .ms1Pin = 11,
        .ms2Pin = 12
    };

#elif defined(ESP32)
    const PinConfig TMC2209_PINS = {
        .stepPin = 4,
        .dirPin = 2,
        .enablePin = 5,
        .ms1Pin = 17,
        .ms2Pin = 16
    };
    
    const PinConfig ULN2003_PINS = {
        .stepPin = 19,
        .dirPin = 18,
        .enablePin = 5,
        .ms1Pin = 17,
        .ms2Pin = 16
    };
#endif

// Motor configuration parameters
struct MotorConfig {
    const int stepsPerRev;      // Steps per revolution
    const int microstepping;    // Microstepping multiplier
    const float maxSpeed;       // Maximum speed in steps/second
    const float acceleration;   // Steps/second^2
    const int totalSwingDeg;   // Total degrees of swing motion
};

// Predefined motor configurations
const MotorConfig NEMA11_CONFIG = {
    .stepsPerRev = 200,     // NEMA 11 standard
    .microstepping = 16,    // TMC2209 16x microstepping
    .maxSpeed = 2000.0,     // Steps per second
    .acceleration = 1000.0, // Steps per second squared
    .totalSwingDeg = 90     // Total swing arc
};

const MotorConfig BYUJ28_CONFIG = {
    .stepsPerRev = 2048,    // 28BYJ-48 standard
    .microstepping = 1,     // ULN2003 no microstepping
    .maxSpeed = 1000.0,     // Lower speed due to motor characteristics
    .acceleration = 500.0,  // Lower acceleration
    .totalSwingDeg = 44     // Same swing arc
};