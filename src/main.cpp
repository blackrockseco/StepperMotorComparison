// src/main.cpp
#include <Arduino.h>
#include "board_config.h"
#include "motor_config.h"
#include "stepper_driver.h"

// Configuration selection - Comment/uncomment to switch setups
#define USE_TMC2209_NEMA11  // Comment this line to use ULN2003 + 28BYJ-48

// Create the appropriate driver instance based on configuration
#ifdef USE_TMC2209_NEMA11
   StepperDriver driver(NEMA11_CONFIG, TMC2209_PINS, true);
#else
   StepperDriver driver(BYUJ28_CONFIG, ULN2003_PINS, false);
#endif

void setup() {
   // Initialize serial communication
   Serial.begin(SERIAL_BAUD);
   
   // Print startup information
   Serial.println("\n=== Stepper Motor Comparison Test ===");
   Serial.print("Board: ");
   Serial.println(BOARD_NAME);
   
   #ifdef USE_TMC2209_NEMA11
       Serial.println("Configuration: NEMA 11 with TMC2209");
   #else
       Serial.println("Configuration: 28BYJ-48 with ULN2003");
   #endif
   
   // Initialize the stepper driver
   driver.setup();
   
   Serial.println("Setup complete - Starting oscillation test");
   Serial.println("========================================");
}

void loop() {
   // Execute the oscillating motion
   driver.runOscillation();
}