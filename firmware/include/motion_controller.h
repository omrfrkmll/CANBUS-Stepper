#pragma once

#include <Arduino.h>
#include "config.h"
#include "modes.h"
#include "encoder_as5600.h"
#include "tmc2209_handler.h"

struct PIDGains {
    float kp;
    float ki;
    float kd;
    float max_integral;
    float max_output;
};

class MotionController {
public:
    MotionController(TMC2209Handler &driver, EncoderAS5600 &encoder);

    void begin();
    void update(); // Called at high frequency (1 kHz PID loop)

    void setControlMode(ControlMode mode);
    void setTargetPosition(float target_deg);
    void setTargetVelocity(float target_deg_s);
    void setPIDGains(float kp, float ki, float kd);

    void enable(bool state);
    void emergencyStop();

    float getTargetPosition();
    float getTargetVelocity();
    float getPositionError();
    int32_t getStepCount();
    ControlMode getControlMode();

private:
    TMC2209Handler &_driver;
    EncoderAS5600 &_encoder;

    ControlMode _control_mode;
    PIDGains _pid;

    float _target_pos_deg;
    float _target_vel_deg_s;
    float _current_setpoint_deg;

    float _integral_error;
    float _last_error;
    float _last_measured_pos;
    uint32_t _last_loop_us;

    int32_t _step_count;
    int8_t _direction;

    void generateSteps(float velocity_deg_s, float dt);
    void singleStep(bool forward);
};
