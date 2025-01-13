// src/stepper_driver.cpp
#include "stepper_driver.h"
#include <Arduino.h>

StepperDriver::StepperDriver(const MotorConfig& config, const PinConfig& pins, bool isTMC2209) 
   : motorConfig(config)
   , pinConfig(pins)
   , isTMC2209Driver(isTMC2209)
   , movingClockwise(true) {
   
   if (isTMC2209Driver) {
       stepper = new AccelStepper(AccelStepper::DRIVER, pinConfig.stepPin, pinConfig.dirPin);
   } else {
       // For ULN2003, using 4-wire stepper configuration
       stepper = new AccelStepper(AccelStepper::FULL4WIRE, 
           pinConfig.stepPin, pinConfig.dirPin, 
           pinConfig.enablePin, pinConfig.ms1Pin);
   }
   calculateSteps();
}

void StepperDriver::calculateSteps() {
   float halfSwingDeg = motorConfig.totalSwingDeg / 2.0;
   stepsForHalfSwing = (motorConfig.stepsPerRev * motorConfig.microstepping * (halfSwingDeg / 360.0));
   
   Serial.print("Calculated steps for half swing: ");
   Serial.println(stepsForHalfSwing);
}

void StepperDriver::setup() {
   Serial.print("Setting up ");
   Serial.println(isTMC2209Driver ? "TMC2209 driver" : "ULN2003 driver");

   if (isTMC2209Driver) {
       setupTMC2209();
   } else {
       setupULN2003();
   }
   
   configureDriver();
   
   Serial.println("Driver setup complete");
}

void StepperDriver::setupTMC2209() {
   pinMode(pinConfig.enablePin, OUTPUT);
   pinMode(pinConfig.ms1Pin, OUTPUT);
   pinMode(pinConfig.ms2Pin, OUTPUT);
   
   digitalWrite(pinConfig.enablePin, LOW);    // Enable driver (active low)
   digitalWrite(pinConfig.ms1Pin, HIGH);      // Set 16x microstepping
   digitalWrite(pinConfig.ms2Pin, HIGH);      // Set 16x microstepping
}

void StepperDriver::setupULN2003() {
   // ULN2003 doesn't require special setup
   // Pins are configured by AccelStepper constructor
}

void StepperDriver::configureDriver() {
   stepper->setMaxSpeed(motorConfig.maxSpeed);
   stepper->setAcceleration(motorConfig.acceleration);
   stepper->setCurrentPosition(0);
}

void StepperDriver::runOscillation() {
   if (stepper->distanceToGo() == 0) {
       Serial.println("Reversing direction");
       delay(2000);  // Pause at endpoints
       
       int targetPosition = movingClockwise ? stepsForHalfSwing : -stepsForHalfSwing;
       stepper->moveTo(targetPosition);
       movingClockwise = !movingClockwise;
   }
   
   stepper->run();
}

void StepperDriver::stopMotion() {
   stepper->stop();
   stepper->setCurrentPosition(stepper->currentPosition());  // Prevents further movement
}

float StepperDriver::getCurrentPosition() {
   return stepper->currentPosition();
}

float StepperDriver::getTargetPosition() {
   return stepper->targetPosition();
}

bool StepperDriver::isMoving() {
   return stepper->isRunning();
}