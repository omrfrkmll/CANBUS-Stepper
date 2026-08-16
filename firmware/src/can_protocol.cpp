#include "can_protocol.h"
#include <string.h>

CANBusProtocol::CANBusProtocol(MotionController &motion, EncoderAS5600 &encoder, PowerAndThermalMonitor &monitor, TMC2209Handler &driver)
    : _motion(motion),
      _encoder(encoder),
      _monitor(monitor),
      _driver(driver),
      _node_id(DEFAULT_CAN_NODE_ID),
      _rx_count(0),
      _tx_count(0),
      _last_telemetry_ms(0),
      _is_installed(false) {}

bool CANBusProtocol::begin(uint32_t baudrate, uint32_t node_id) {
    _node_id = node_id;

    // Enable TCAN1051V Transceiver (CAN_STB LOW = Normal Active Mode)
    pinMode(PIN_CAN_STB, OUTPUT);
    digitalWrite(PIN_CAN_STB, LOW);

    // General TWAI Configuration
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        (gpio_num_t)PIN_CAN_TX,
        (gpio_num_t)PIN_CAN_RX,
        TWAI_MODE_NORMAL
    );
    g_config.tx_queue_len = 20;
    g_config.rx_queue_len = 20;

    // Timing Configuration
    twai_timing_config_t t_config;
    if (baudrate == 1000000) {
        t_config = TWAI_TIMING_CONFIG_1MBITS();
    } else if (baudrate == 500000) {
        t_config = TWAI_TIMING_CONFIG_500KBITS();
    } else if (baudrate == 250000) {
        t_config = TWAI_TIMING_CONFIG_250KBITS();
    } else {
        t_config = TWAI_TIMING_CONFIG_500KBITS();
    }

    // Filter Configuration (Accept all frames by default)
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    // Install and Start Driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        if (twai_start() == ESP_OK) {
            _is_installed = true;
            return true;
        }
    }

    return false;
}

void CANBusProtocol::update() {
    if (!_is_installed) return;

    twai_message_t rx_msg;
    while (twai_receive(&rx_msg, 0) == ESP_OK) {
        _rx_count++;
        processIncomingFrame(rx_msg);
    }

    // Broadcast periodic telemetry (every 20ms = 50 Hz)
    uint32_t now_ms = millis();
    if (now_ms - _last_telemetry_ms >= 20) {
        _last_telemetry_ms = now_ms;
        sendTelemetry();
    }
}

void CANBusProtocol::processIncomingFrame(const twai_message_t &msg) {
    uint32_t cmd_type = msg.identifier & 0x0F; // Lower 4 bits
    uint32_t target_id = msg.identifier & ~0x0F;

    // Check if message is for this node or broadcast
    if (target_id != _node_id && target_id != 0x00) {
        return;
    }

    switch (cmd_type) {
        case CAN_CMD_SET_TARGET_POS: {
            if (msg.data_length_code >= 4) {
                float target_pos;
                memcpy(&target_pos, msg.data, sizeof(float));
                _motion.setTargetPosition(target_pos);
                _motion.setControlMode(ControlMode::CLOSED_LOOP_POSITION);
            }
            break;
        }

        case CAN_CMD_SET_TARGET_VEL: {
            if (msg.data_length_code >= 4) {
                float target_vel;
                memcpy(&target_vel, msg.data, sizeof(float));
                _motion.setTargetVelocity(target_vel);
                _motion.setControlMode(ControlMode::CLOSED_LOOP_VELOCITY);
            }
            break;
        }

        case CAN_CMD_SET_CURRENT: {
            if (msg.data_length_code >= 2) {
                uint16_t current_ma;
                memcpy(&current_ma, msg.data, sizeof(uint16_t));
                _driver.setCurrent(current_ma);
            }
            break;
        }

        case CAN_CMD_ENABLE_MOTOR: {
            if (msg.data_length_code >= 1) {
                uint8_t state = msg.data[0];
                if (state == 1) {
                    _motion.enable(true);
                } else if (state == 2) {
                    _motion.emergencyStop();
                } else {
                    _motion.enable(false);
                }
            }
            break;
        }

        case CAN_CMD_SET_ZERO: {
            _encoder.setZeroPosition();
            _motion.setTargetPosition(0.0f);
            break;
        }

        case CAN_CMD_SET_PID: {
            if (msg.data_length_code >= 6) {
                uint16_t kp_raw, ki_raw, kd_raw;
                memcpy(&kp_raw, &msg.data[0], 2);
                memcpy(&ki_raw, &msg.data[2], 2);
                memcpy(&kd_raw, &msg.data[4], 2);
                _motion.setPIDGains(kp_raw / 100.0f, ki_raw / 1000.0f, kd_raw / 100.0f);
            }
            break;
        }

        case CAN_CMD_REQ_TELEMETRY: {
            sendTelemetry();
            break;
        }

        default:
            break;
    }
}

void CANBusProtocol::sendTelemetry() {
    if (!_is_installed) return;

    // Frame 1: Actual Angle (float 4B) + Velocity (float 4B)
    twai_message_t tx_frame1 = {0};
    tx_frame1.identifier = _node_id | CAN_RESP_TELEMETRY_POS;
    tx_frame1.flags = TWAI_MSG_FLAG_NONE;
    tx_frame1.data_length_code = 8;

    float angle = _encoder.getMultiTurnAngle() * DEG_PER_COUNT;
    float velocity = _encoder.getVelocityDegPerSec();
    memcpy(&tx_frame1.data[0], &angle, 4);
    memcpy(&tx_frame1.data[4], &velocity, 4);
    if (twai_transmit(&tx_frame1, 0) == ESP_OK) {
        _tx_count++;
    }

    // Frame 2: VBUS (uint16_t mV) + Temp (int16_t 0.1C) + Status Flags (uint32_t)
    twai_message_t tx_frame2 = {0};
    tx_frame2.identifier = _node_id | CAN_RESP_TELEMETRY_STAT;
    tx_frame2.flags = TWAI_MSG_FLAG_NONE;
    tx_frame2.data_length_code = 8;

    uint16_t vbus_mv = (uint16_t)(_monitor.getVbusVoltage() * 1000.0f);
    int16_t temp_tenth_c = (int16_t)(_monitor.getTemperatureC() * 10.0f);
    uint32_t status_flags = 0;
    if (_driver.isStalled()) status_flags |= (1 << 0);
    if (_motion.getControlMode() != ControlMode::MOTOR_DISABLED) status_flags |= (1 << 1);
    if (_monitor.isOverheated()) status_flags |= (1 << 2);
    if (_monitor.isUndervoltage()) status_flags |= (1 << 3);

    memcpy(&tx_frame2.data[0], &vbus_mv, 2);
    memcpy(&tx_frame2.data[2], &temp_tenth_c, 2);
    memcpy(&tx_frame2.data[4], &status_flags, 4);
    if (twai_transmit(&tx_frame2, 0) == ESP_OK) {
        _tx_count++;
    }
}

uint32_t CANBusProtocol::getRxCount() {
    return _rx_count;
}

uint32_t CANBusProtocol::getTxCount() {
    return _tx_count;
}

bool CANBusProtocol::isBusHealthy() {
    if (!_is_installed) return false;
    twai_status_info_t status;
    if (twai_get_status_info(&status) == ESP_OK) {
        return (status.state == TWAI_STATE_RUNNING);
    }
    return false;
}
