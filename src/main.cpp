// src/main.cpp
#include "board_config.h"
#include "stepper_driver.h"
#include "wifi_config.h"

#include "pin_config.h"
#include "motor_config.h"

// Create instances of pin configurations
TMC2209PinConfig tmc2209PinConfig(
    TMC2209_STEP_PIN,
    TMC2209_DIR_PIN,
    TMC2209_ENABLE_PIN,
    TMC2209_MS1_PIN,
    TMC2209_MS2_PIN
);

ULN2003PinConfig uln2003PinConfig(
    ULN2003_IN1_PIN,
    ULN2003_IN2_PIN,
    ULN2003_IN3_PIN,
    ULN2003_IN4_PIN
);
StepperDriver stepperDriver1(tmc2209Pins, uln2003Pins, true);  // NEMA 11 + TMC2209
StepperDriver stepperDriver2(tmc2209Pins, uln2003Pins, false);  // 28BYJ-48 + ULN2003

void setup() {
    Serial.begin(SERIAL_BAUD);
    if (strcmp(BOARD_NAME, "ESP32") == 0) {
        WifiManager::setupWiFiAndOTA();  // Only on ESP32
    }

    stepperDriver1.setup();  // Setup NEMA 11
    stepperDriver2.setup();  // Setup 28BYJ-48
    Serial.println("Setup complete!");
}

void loop() {
    if (strcmp(BOARD_NAME, "ESP32") == 0) {
        WifiManager::handle();  // Handle OTA on ESP32
    }

    stepperDriver1.runOscillation();
    stepperDriver2.runOscillation();
}

// src/stepper_driver.cpp
#include "stepper_driver.h"

StepperDriver::StepperDriver(const TMC2209PinConfig& tmcPins, const ULN2003PinConfig& ulnPins, bool isTMC2209)
    : tmcPinConfig(tmcPins), ulnPinConfig(ulnPins), isTMC2209Driver(isTMC2209), stepper(nullptr), movingClockwise(true), stepsForHalfSwing(0) {}

void StepperDriver::setup() {
    if (isTMC2209Driver) {
        stepper = new AccelStepper(AccelStepper::DRIVER, tmcPinConfig.stepPin, tmcPinConfig.dirPin);
        setupTMC2209();
    } else {
        stepper = new AccelStepper(AccelStepper::FULL4WIRE, uln2003PinConfig.in1Pin, uln2003PinConfig.in3Pin, uln2003PinConfig.in2Pin, uln2003PinConfig.in4Pin);
        setupULN2003();
    }

    calculateSteps();
    stepper->setMaxSpeed(isTMC2209Driver ? DEFAULT_MAX_SPEED_NEMA : DEFAULT_MAX_SPEED_BYJ);
    stepper->setAcceleration(isTMC2209Driver ? DEFAULT_ACCELERATION_NEMA : DEFAULT_ACCELERATION_BYJ);
}

void StepperDriver::runOscillation() {
    if (stepper->distanceToGo() == 0) {
        delay(2000);
        int targetPosition = movingClockwise ? stepsForHalfSwing : -stepsForHalfSwing;
        stepper->moveTo(targetPosition);
        movingClockwise = !movingClockwise;
    }
    stepper->run();
}

void StepperDriver::stopMotion() {
    if (stepper) {
        stepper->stop();
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
}

void StepperDriver::setupTMC2209() {
    pinMode(tmcPinConfig.enablePin, OUTPUT);
    digitalWrite(tmcPinConfig.enablePin, LOW);  // Enable driver
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