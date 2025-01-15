// include/stepper_driver.h
#pragma once
#include <AccelStepper.h>
#include "motor_config.h"
#include "board_config.h"
#include "pin_config.h"  // Ensure the pin configurations are included

class StepperDriver {
public:
   // Constructor takes both motor and pin configurations plus driver type
   StepperDriver(const TMC2209PinConfig& tmcPins, const ULN2003PinConfig& ulnPins, bool isTMC2209);

   // Core functions
   void setup();                 // Initialize the stepper motor and driver
   void runOscillation();         // Execute the oscillating motion
   void stopMotion();             // Emergency stop function

   // Configuration getters
   float getCurrentPosition();    // Get current position in steps
   float getTargetPosition();     // Get target position in steps
   bool isMoving();               // Check if motor is currently moving

private:
   // Hardware objects/configurations
   AccelStepper* stepper;
   const TMC2209PinConfig& tmcPinConfig;
   const ULN2003PinConfig& ulnPinConfig;
   const bool isTMC2209Driver;

   // Motion state
   bool movingClockwise;
   int stepsForHalfSwing;

   // Private helper functions
   void calculateSteps();         // Calculate steps for the swing motion
   void setupTMC2209();           // Configure TMC2209 specific settings
   void setupULN2003();           // Configure ULN2003 specific settings
   void configureDriver();        // Set up driver parameters
};
