#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "config.h"

struct AS5600Status {
    bool magnet_detected;
    bool magnet_too_weak;
    bool magnet_too_strong;
    uint8_t gain_agc;
    uint16_t magnitude;
};

class EncoderAS5600 {
public:
    EncoderAS5600();

    bool begin(TwoWire &wire = Wire);
    void update();

    uint16_t getRawAngle();
    float getAngleDegrees();
    int32_t getMultiTurnAngle();
    float getVelocityDegPerSec();
    AS5600Status getStatus();

    void setZeroPosition();
    bool isMagnetValid();

private:
    TwoWire *_wire;
    uint16_t _last_raw_angle;
    int32_t _multi_turn_count;
    int32_t _last_multi_turn_angle;
    float _filtered_velocity;
    uint32_t _last_update_us;
    uint16_t _zero_offset;

    uint16_t readRegister16(uint8_t reg);
    uint8_t readRegister8(uint8_t reg);
};
