// src/main.cpp
#include <Arduino.h>
#include "board_config.h"
#include "motor_config.h"
#include "stepper_driver.h"

#if defined(ESP32)
    #include "wifi_config.h"
#endif

// Configuration selection
#define USE_TMC2209_NEMA11

#ifdef USE_TMC2209_NEMA11
    StepperDriver driver(NEMA11_CONFIG, TMC2209_PINS, true);
#else
    StepperDriver driver(BYUJ28_CONFIG, ULN2003_PINS, false);
#endif

void setup() {
    Serial.begin(SERIAL_BAUD);
    
    Serial.println("\n=== Stepper Motor Comparison Test ===");
    Serial.print("Board: ");
    Serial.println(BOARD_NAME);
    
    #if defined(ESP32)
        if (WifiManager::setupWiFiAndOTA()) {
            Serial.println("WiFi and OTA setup successful");
        } else {
            Serial.println("WiFi setup failed, continuing without OTA");
        }
    #endif

    #ifdef USE_TMC2209_NEMA11
        Serial.println("Configuration: NEMA 11 with TMC2209");
    #else
        Serial.println("Configuration: 28BYJ-48 with ULN2003");
    #endif
    
    driver.setup();
    Serial.println("Setup complete - Starting oscillation test");
    Serial.println("========================================");
}

void loop() {
    #if defined(ESP32)
        WifiManager::handle();  // Handle OTA updates
    #endif
    
    driver.runOscillation();
}