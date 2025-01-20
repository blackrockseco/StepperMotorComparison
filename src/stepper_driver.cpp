// stepper_driver.cpp
#include "stepper_driver.h"
#include <Arduino.h>

// Constructors
StepperDriver::StepperDriver(const TMC2209PinConfig& tmcPins, bool isTMC2209)
    : stepper(nullptr)
    , tmcPinConfig(tmcPins)
    , isTMC2209Driver(isTMC2209)
    , movingClockwise(true)
    , stepsForHalfSwing(0) {
}

StepperDriver::StepperDriver(const ULN2003PinConfig& ulnPins, bool isTMC2209)
    : stepper(nullptr)
    , ulnPinConfig(ulnPins)
    , isTMC2209Driver(isTMC2209)
    , movingClockwise(true)
    , stepsForHalfSwing(0) {
}

void StepperDriver::setup() {
    if (isTMC2209Driver) {
        stepper = new AccelStepper(AccelStepper::DRIVER, tmcPinConfig.stepPin, tmcPinConfig.dirPin);
        setupTMC2209();
    } else {
        stepper = new AccelStepper(AccelStepper::FULL4WIRE, 
            ulnPinConfig.in1Pin, 
            ulnPinConfig.in2Pin, 
            ulnPinConfig.in3Pin, 
            ulnPinConfig.in4Pin);
        setupULN2003();
    }
    calculateSteps();
    configureDriver();
    stepper->setCurrentPosition(0);
    Serial.println(isTMC2209Driver ? "TMC2209 motor initialized" : "ULN2003 motor initialized");
}

void StepperDriver::runOscillation() {
    if (!stepper) return;
    
    if (!stepper->isRunning()) {
        long currentPos = stepper->currentPosition();
        
        if (currentPos == 0) {
            stepper->moveTo(stepsForHalfSwing);
        } else if (currentPos >= stepsForHalfSwing) {
            stepper->moveTo(-stepsForHalfSwing);
        } else if (currentPos <= -stepsForHalfSwing) {
            stepper->moveTo(0);
        }
    }
    
    stepper->run();
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
}

void StepperDriver::setupULN2003() {
    // No special setup needed for ULN2003—AccelStepper handles step sequences
    Serial.println("ULN2003 driver configured.");
}

void StepperDriver::configureDriver() {
    const float DEFAULT_MAX_SPEED_NEMA = 1000;    // Steps per second
    const float DEFAULT_MAX_SPEED_BYJ = 500;      // Reduced from 1000
    const float DEFAULT_ACCELERATION_NEMA = 500;   
    const float DEFAULT_ACCELERATION_BYJ = 250;    // Reduced from 500fd

    stepper->setMaxSpeed(isTMC2209Driver ? DEFAULT_MAX_SPEED_NEMA : DEFAULT_MAX_SPEED_BYJ);
    stepper->setAcceleration(isTMC2209Driver ? DEFAULT_ACCELERATION_NEMA : DEFAULT_ACCELERATION_BYJ);
}