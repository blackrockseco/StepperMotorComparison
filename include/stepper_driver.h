// include/stepper_driver.h
#pragma once
#include <AccelStepper.h>
#include "motor_config.h"
#include "board_config.h"
#include "pin_config.h"  // Ensure the pin configurations are included

class StepperDriver {
public:
   StepperDriver(const TMC2209PinConfig& tmcPins, bool isTMC2209);
   StepperDriver(const ULN2003PinConfig& ulnPins, bool isTMC2209);

   void setup();
   void runOscillation();
   void stopMotion();
   float getCurrentPosition();
   float getTargetPosition();
   bool isMoving();

private:
   AccelStepper* stepper;
   const TMC2209PinConfig* tmcPinConfig;
   const ULN2003PinConfig* ulnPinConfig;
   bool isTMC2209Driver;
   bool movingClockwise;
   int stepsForHalfSwing;

   void calculateSteps();
   void setupTMC2209();
   void setupULN2003();
   void configureDriver();
};
