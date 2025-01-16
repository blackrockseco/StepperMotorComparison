#include "board_config.h"
#include "pin_config.h"
#include "motor_config.h"
#include "stepper_driver.h"

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

// Create stepper driver objects using the correct constructor
StepperDriver nema11Driver(tmc2209PinConfig, true);  // NEMA 11 + TMC2209
StepperDriver byjDriver(uln2003PinConfig, false);    // 28BYJ-48 + ULN2003

void setup() {
    Serial.begin(115200);

    nema11Driver.setup();  // Setup NEMA 11
    byjDriver.setup();     // Setup 28BYJ-48
}

void loop() {
    nema11Driver.runOscillation();
    byjDriver.runOscillation();
}
