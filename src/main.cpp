// src/main.cpp
#include <Arduino.h>
#include "board_config.h"
#include "motor_config.h"
#include "stepper_driver.h"

#if defined(ESP32)
    #include "wifi_config.h"
#endif

// Define pin configurations for each motor setup
const TMC2209PinConfig NEMA11_PINS(
    4,  // stepPin
    2,  // dirPin
    5,  // enablePin
    17,  // ms1Pin
    16   // ms2Pin
);

const ULN2003PinConfig BYJ48_PINS(
    33,  // IN1
    32,  // IN2
    35,  // IN3
    34  // IN4

);

// Create stepper driver instances with explicit pin configurations
StepperDriver nema11Driver(NEMA11_CONFIG, NEMA11_PINS, true);   // TMC2209 + NEMA11
StepperDriver byjDriver(BYUJ28_CONFIG, BYJ48_PINS, false);      // ULN2003 + 28BYJ-48

void setup() {
    Serial.begin(SERIAL_BAUD);
    
    Serial.println("\n=== Dual Stepper Motor Comparison Test ===");
    Serial.print("Board: ");
    Serial.println(BOARD_NAME);
    
    #if defined(ESP32)
        if (WifiManager::setupWiFiAndOTA()) {
            Serial.println("WiFi and OTA setup successful");
        } else {
            Serial.println("WiFi setup failed, continuing without OTA");
        }
    #endif

    // Initialize motors
    Serial.println("\nInitializing NEMA11 with TMC2209");
    Serial.printf("Pins - Step:%d, Dir:%d, Enable:%d, MS1:%d, MS2:%d\n",
        NEMA11_PINS.stepPin, NEMA11_PINS.dirPin, NEMA11_PINS.enablePin,
        NEMA11_PINS.ms1Pin, NEMA11_PINS.ms2Pin);
    nema11Driver.setup();
    
    Serial.println("\nInitializing 28BYJ-48 with ULN2003");
    Serial.printf("Pins - Step:%d, Dir:%d, Enable:%d, MS1:%d, MS2:%d\n",
        BYJ48_PINS.stepPin, BYJ48_PINS.dirPin, BYJ48_PINS.enablePin,
        BYJ48_PINS.ms1Pin, BYJ48_PINS.ms2Pin);
    byjDriver.setup();
    
    Serial.println("\nSetup complete - Starting oscillation test");
    Serial.println("==========================================");
}

void loop() {
    #if defined(ESP32)
        WifiManager::handle();  // Handle OTA updates
    #endif
    
    // Run both motors
    nema11Driver.runOscillation();
    byjDriver.runOscillation();
}