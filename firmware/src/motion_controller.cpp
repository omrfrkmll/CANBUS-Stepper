#include "motion_controller.h"
#include <math.h>

MotionController::MotionController(TMC2209Handler &driver, EncoderAS5600 &encoder)
    : _driver(driver),
      _encoder(encoder),
      _control_mode(ControlMode::MOTOR_DISABLED),
      _target_pos_deg(0.0f),
      _target_vel_deg_s(0.0f),
      _current_setpoint_deg(0.0f),
      _integral_error(0.0f),
      _last_error(0.0f),
      _last_measured_pos(0.0f),
      _last_loop_us(0),
      _step_count(0),
      _direction(1) {
    _pid.kp = PID_KP;
    _pid.ki = PID_KI;
    _pid.kd = PID_KD;
    _pid.max_integral = 500.0f;
    _pid.max_output = 3600.0f; // Max deg/s output velocity
}

void MotionController::begin() {
    pinMode(PIN_TMC_STEP, OUTPUT);
    pinMode(PIN_TMC_DIR, OUTPUT);
    digitalWrite(PIN_TMC_STEP, LOW);
    digitalWrite(PIN_TMC_DIR, LOW);

    _last_loop_us = micros();
    _last_measured_pos = _encoder.getMultiTurnAngle() * DEG_PER_COUNT;
    _target_pos_deg = _last_measured_pos;
    _current_setpoint_deg = _last_measured_pos;
}

void MotionController::setControlMode(ControlMode mode) {
    _control_mode = mode;
    _integral_error = 0.0f;
    _last_error = 0.0f;
    if (mode == ControlMode::MOTOR_DISABLED) {
        _driver.enable(false);
    } else {
        _driver.enable(true);
    }
}

void MotionController::setTargetPosition(float target_deg) {
    _target_pos_deg = target_deg;
}

void MotionController::setTargetVelocity(float target_deg_s) {
    _target_vel_deg_s = target_deg_s;
}

void MotionController::setPIDGains(float kp, float ki, float kd) {
    _pid.kp = kp;
    _pid.ki = ki;
    _pid.kd = kd;
}

void MotionController::enable(bool state) {
    if (state) {
        setControlMode(ControlMode::CLOSED_LOOP_POSITION);
    } else {
        setControlMode(ControlMode::MOTOR_DISABLED);
    }
}

void MotionController::emergencyStop() {
    _target_vel_deg_s = 0.0f;
    _target_pos_deg = _encoder.getMultiTurnAngle() * DEG_PER_COUNT;
    _current_setpoint_deg = _target_pos_deg;
    _integral_error = 0.0f;
    setControlMode(ControlMode::TORQUE_HOLD);
}

float MotionController::getTargetPosition() {
    return _target_pos_deg;
}

float MotionController::getTargetVelocity() {
    return _target_vel_deg_s;
}

float MotionController::getPositionError() {
    float measured = _encoder.getMultiTurnAngle() * DEG_PER_COUNT;
    return _target_pos_deg - measured;
}

int32_t MotionController::getStepCount() {
    return _step_count;
}

ControlMode MotionController::getControlMode() {
    return _control_mode;
}

void MotionController::singleStep(bool forward) {
    if (forward) {
        if (_direction != 1) {
            digitalWrite(PIN_TMC_DIR, HIGH);
            _direction = 1;
            delayMicroseconds(1);
        }
        _step_count++;
    } else {
        if (_direction != -1) {
            digitalWrite(PIN_TMC_DIR, LOW);
            _direction = -1;
            delayMicroseconds(1);
        }
        _step_count--;
    }

    digitalWrite(PIN_TMC_STEP, HIGH);
    delayMicroseconds(2);
    digitalWrite(PIN_TMC_STEP, LOW);
}

void MotionController::update() {
    if (_control_mode == ControlMode::MOTOR_DISABLED) {
        return;
    }

    uint32_t now_us = micros();
    float dt = (now_us - _last_loop_us) / 1000000.0f;
    if (dt <= 0.0f || dt > 0.05f) dt = 0.001f;
    _last_loop_us = now_us;

    _encoder.update();
    float measured_pos = _encoder.getMultiTurnAngle() * DEG_PER_COUNT;

    float command_velocity = 0.0f;

    switch (_control_mode) {
        case ControlMode::CLOSED_LOOP_POSITION: {
            float error = _target_pos_deg - measured_pos;
            _integral_error += error * dt;
            if (_integral_error > _pid.max_integral) _integral_error = _pid.max_integral;
            if (_integral_error < -_pid.max_integral) _integral_error = -_pid.max_integral;

            float derivative = (error - _last_error) / dt;
            _last_error = error;

            // PID calculation
            command_velocity = (_pid.kp * error) + (_pid.ki * _integral_error) + (_pid.kd * derivative);

            // Clamp maximum output velocity
            if (command_velocity > _pid.max_output) command_velocity = _pid.max_output;
            if (command_velocity < -_pid.max_output) command_velocity = -_pid.max_output;

            // Deadband to prevent jitter when positioned accurately
            if (fabs(error) < 0.05f) {
                command_velocity = 0.0f;
            }
            break;
        }

        case ControlMode::CLOSED_LOOP_VELOCITY: {
            float measured_vel = _encoder.getVelocityDegPerSec();
            float error_vel = _target_vel_deg_s - measured_vel;
            command_velocity = _target_vel_deg_s + (_pid.kp * error_vel * 0.2f);
            break;
        }

        case ControlMode::OPEN_LOOP_VELOCITY: {
            command_velocity = _target_vel_deg_s;
            break;
        }

        case ControlMode::TORQUE_HOLD: {
            command_velocity = 0.0f;
            break;
        }

        default:
            command_velocity = 0.0f;
            break;
    }

    if (fabs(command_velocity) > 0.1f) {
        generateSteps(command_velocity, dt);
    }
}

void MotionController::generateSteps(float velocity_deg_s, float dt) {
    // Convert deg/s to steps/s
    float steps_per_deg = (float)STEPS_PER_REV_NEMA / 360.0f;
    float steps_to_run = velocity_deg_s * steps_per_deg * dt;

    int32_t count = (int32_t)fabs(steps_to_run);
    bool forward = (velocity_deg_s >= 0.0f);

    for (int32_t i = 0; i < count; i++) {
        singleStep(forward);
    }
}
