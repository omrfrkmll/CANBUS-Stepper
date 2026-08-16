#pragma once

#include <Arduino.h>
#include <TMCStepper.h>
#include "config.h"

struct TMCStatus {
    bool is_communicating;
    bool overtemperature;
    bool overtemperature_prewarning;
    bool short_to_ground_a;
    bool short_to_ground_b;
    bool open_load_a;
    bool open_load_b;
    bool stand_still;
    uint16_t sg_result;
};

class TMC2209Handler {
public:
    TMC2209Handler();

    bool begin();
    void enable(bool state = true);
    void setCurrent(uint16_t current_ma);
    void setMicrosteps(uint16_t microsteps);
    void setStealthChop(bool enable);
    void setStallGuardThreshold(uint8_t threshold);

    TMCStatus readStatus();
    bool isStalled();

private:
    TMC2209Stepper _driver;
    bool _is_enabled;
    uint16_t _current_ma;
    uint16_t _microsteps;
};
