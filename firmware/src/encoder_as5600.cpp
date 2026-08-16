#include "encoder_as5600.h"

#define AS5600_REG_STATUS       0x0B
#define AS5600_REG_RAW_ANGLE    0x0C
#define AS5600_REG_ANGLE        0x0E
#define AS5600_REG_AGC          0x1A
#define AS5600_REG_MAGNITUDE    0x1B

EncoderAS5600::EncoderAS5600()
    : _wire(&Wire),
      _last_raw_angle(0),
      _multi_turn_count(0),
      _last_multi_turn_angle(0),
      _filtered_velocity(0.0f),
      _last_update_us(0),
      _zero_offset(0) {}

bool EncoderAS5600::begin(TwoWire &wire) {
    _wire = &wire;
    _wire->begin(PIN_I2C_SDA, PIN_I2C_SCL, I2C_FREQUENCY);

    // Initial read
    uint16_t angle = getRawAngle();
    _last_raw_angle = angle;
    _last_update_us = micros();

    AS5600Status status = getStatus();
    return status.magnet_detected;
}

uint16_t EncoderAS5600::readRegister16(uint8_t reg) {
    _wire->beginTransmission(AS5600_I2C_ADDR);
    _wire->write(reg);
    if (_wire->endTransmission(false) != 0) {
        return _last_raw_angle;
    }

    if (_wire->requestFrom((uint8_t)AS5600_I2C_ADDR, (uint8_t)2) == 2) {
        uint8_t msb = _wire->read();
        uint8_t lsb = _wire->read();
        return ((uint16_t)(msb & 0x0F) << 8) | lsb;
    }
    return _last_raw_angle;
}

uint8_t EncoderAS5600::readRegister8(uint8_t reg) {
    _wire->beginTransmission(AS5600_I2C_ADDR);
    _wire->write(reg);
    if (_wire->endTransmission(false) != 0) {
        return 0;
    }

    if (_wire->requestFrom((uint8_t)AS5600_I2C_ADDR, (uint8_t)1) == 1) {
        return _wire->read();
    }
    return 0;
}

uint16_t EncoderAS5600::getRawAngle() {
    return readRegister16(AS5600_REG_RAW_ANGLE);
}

void EncoderAS5600::update() {
    uint32_t now_us = micros();
    float dt = (now_us - _last_update_us) / 1000000.0f;
    if (dt <= 0.0f) dt = 0.001f;

    uint16_t current_raw = getRawAngle();
    int32_t diff = (int32_t)current_raw - (int32_t)_last_raw_angle;

    // Handle wrap-around (4096 counts per revolution)
    if (diff > (ENCODER_RESOLUTION / 2)) {
        _multi_turn_count--;
    } else if (diff < -(ENCODER_RESOLUTION / 2)) {
        _multi_turn_count++;
    }

    _last_raw_angle = current_raw;
    _last_update_us = now_us;

    int32_t current_multi_turn = (_multi_turn_count * ENCODER_RESOLUTION) + current_raw - _zero_offset;
    float instant_velocity = ((float)(current_multi_turn - _last_multi_turn_angle) * DEG_PER_COUNT) / dt;

    // Exponential moving average filter for velocity
    _filtered_velocity = (_filtered_velocity * 0.85f) + (instant_velocity * 0.15f);
    _last_multi_turn_angle = current_multi_turn;
}

float EncoderAS5600::getAngleDegrees() {
    uint16_t raw = (_last_raw_angle >= _zero_offset) 
                   ? (_last_raw_angle - _zero_offset) 
                   : (ENCODER_RESOLUTION + _last_raw_angle - _zero_offset);
    return (float)raw * DEG_PER_COUNT;
}

int32_t EncoderAS5600::getMultiTurnAngle() {
    return (_multi_turn_count * ENCODER_RESOLUTION) + _last_raw_angle - _zero_offset;
}

float EncoderAS5600::getVelocityDegPerSec() {
    return _filtered_velocity;
}

AS5600Status EncoderAS5600::getStatus() {
    AS5600Status status;
    uint8_t stat_reg = readRegister8(AS5600_REG_STATUS);
    status.magnet_detected = (stat_reg & 0x20) != 0;
    status.magnet_too_weak = (stat_reg & 0x10) != 0;
    status.magnet_too_strong = (stat_reg & 0x08) != 0;
    status.gain_agc = readRegister8(AS5600_REG_AGC);
    status.magnitude = readRegister16(AS5600_REG_MAGNITUDE);
    return status;
}

void EncoderAS5600::setZeroPosition() {
    _zero_offset = _last_raw_angle;
    _multi_turn_count = 0;
    _last_multi_turn_angle = 0;
}

bool EncoderAS5600::isMagnetValid() {
    AS5600Status s = getStatus();
    return s.magnet_detected && !s.magnet_too_weak && !s.magnet_too_strong;
}
