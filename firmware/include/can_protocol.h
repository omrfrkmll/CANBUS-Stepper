#pragma once

#include <Arduino.h>
#include "driver/twai.h"
#include "config.h"
#include "motion_controller.h"
#include "ntc_sensor.h"
#include "encoder_as5600.h"

// Command IDs (CAN Message Identifier Offset)
#define CAN_CMD_SET_TARGET_POS   0x01
#define CAN_CMD_SET_TARGET_VEL   0x02
#define CAN_CMD_SET_CURRENT      0x03
#define CAN_CMD_ENABLE_MOTOR     0x04
#define CAN_CMD_SET_ZERO         0x05
#define CAN_CMD_SET_PID          0x06
#define CAN_CMD_REQ_TELEMETRY    0x10

#define CAN_RESP_TELEMETRY_POS   0x20
#define CAN_RESP_TELEMETRY_STAT  0x21

class CANBusProtocol {
public:
    CANBusProtocol(MotionController &motion, EncoderAS5600 &encoder, PowerAndThermalMonitor &monitor, TMC2209Handler &driver);

    bool begin(uint32_t baudrate = DEFAULT_CAN_BAUD, uint32_t node_id = DEFAULT_CAN_NODE_ID);
    void update();
    void sendTelemetry();

    uint32_t getRxCount();
    uint32_t getTxCount();
    bool isBusHealthy();

private:
    MotionController &_motion;
    EncoderAS5600 &_encoder;
    PowerAndThermalMonitor &_monitor;
    TMC2209Handler &_driver;

    uint32_t _node_id;
    uint32_t _rx_count;
    uint32_t _tx_count;
    uint32_t _last_telemetry_ms;
    bool _is_installed;

    void processIncomingFrame(const twai_message_t &msg);
};
