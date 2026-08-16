#pragma once

#include <Arduino.h>

enum class OperationMode {
    STANDALONE_CAN,
    DIAGNOSTICS,
    MICROROS,
    KLIPPER_BRIDGE
};

enum class ControlMode {
    OPEN_LOOP_VELOCITY,
    OPEN_LOOP_POSITION,
    CLOSED_LOOP_POSITION,
    CLOSED_LOOP_VELOCITY,
    TORQUE_HOLD,
    MOTOR_DISABLED
};


struct BoardTelemetry {
    float angle_deg;
    int32_t multi_turn_position;
    float velocity_deg_s;
    float vbus_voltage;
    float board_temperature_c;
    uint16_t motor_current_ma;
    bool is_stalled;
    bool is_enabled;
    uint32_t can_rx_count;
    uint32_t can_tx_count;
    uint32_t error_flags;
};
