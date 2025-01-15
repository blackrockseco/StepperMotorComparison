// src/stepper_driver.cpp
#include "stepper_driver.h"
#include <Arduino.h>

void StepperDriver::setup() {
    if (isTMC2209Driver) {
        stepper = new AccelStepper(AccelStepper::DRIVER, tmcPinConfig->stepPin, tmcPinConfig->dirPin);
        setupTMC2209();
    } else {
        stepper = new AccelStepper(AccelStepper::FULL4WIRE, ulnPinConfig->in1Pin, ulnPinConfig->in3Pin, ulnPinConfig->in2Pin, ulnPinConfig->in4Pin);
        setupULN2003();
    }

    calculateSteps();
    stepper->setMaxSpeed(isTMC2209Driver ? DEFAULT_MAX_SPEED_NEMA : DEFAULT_MAX_SPEED_BYJ);
    stepper->setAcceleration(isTMC2209Driver ? DEFAULT_ACCELERATION_NEMA : DEFAULT_ACCELERATION_BYJ);
    stepper->setCurrentPosition(0);
    Serial.println(isTMC2209Driver ? "TMC2209 motor initialized" : "ULN2003 motor initialized");
}


void StepperDriver::stopMotion() {
    if (stepper) {
        stepper->stop();
        stepper->setCurrentPosition(stepper->currentPosition());  // Prevent further movement
        Serial.println("Motion stopped.");
    }
}

float StepperDriver::getCurrentPosition() {
    return stepper ? stepper->currentPosition() : 0;
}

float StepperDriver::getTargetPosition() {
    return stepper ? stepper->targetPosition() : 0;
}

bool StepperDriver::isMoving() {
    return stepper && stepper->isRunning();
}

void StepperDriver::calculateSteps() {
    int stepsPerRevolution = isTMC2209Driver ? 200 * 16 : 2048;  // Microstepping for TMC2209 vs ULN2003 step count
    stepsForHalfSwing = (stepsPerRevolution * (44.0 / 360.0));  // 44-degree arc
    Serial.print("Steps for half swing: ");
    Serial.println(stepsForHalfSwing);
}

void StepperDriver::setupTMC2209() {
    pinMode(tmcPinConfig.enablePin, OUTPUT);
    digitalWrite(tmcPinConfig.enablePin, LOW);  // Enable driver (active low)
    pinMode(tmcPinConfig.ms1Pin, OUTPUT);
    pinMode(tmcPinConfig.ms2Pin, OUTPUT);
    digitalWrite(tmcPinConfig.ms1Pin, HIGH);  // 16x microstepping
    digitalWrite(tmcPinConfig.ms2Pin, HIGH);
    Serial.println("TMC2209 driver configured.");
}

void StepperDriver::setupULN2003() {
    Serial.println("ULN2003 driver configured.");
    // No special enable pins for ULN2003—AccelStepper handles step sequences.
}

void StepperDriver::configureDriver() {
    stepper->setMaxSpeed(isTMC2209Driver ? DEFAULT_MAX_SPEED_NEMA : DEFAULT_MAX_SPEED_BYJ);
    stepper->setAcceleration(isTMC2209Driver ? DEFAULT_ACCELERATION_NEMA : DEFAULT_ACCELERATION_BYJ);
}
