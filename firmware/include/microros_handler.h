#pragma once

#include <Arduino.h>
#include "config.h"
#include "motion_controller.h"
#include "encoder_as5600.h"
#include "ntc_sensor.h"

class MicroROSHandler {
public:
    MicroROSHandler(MotionController &motion, EncoderAS5600 &encoder, PowerAndThermalMonitor &monitor);

    bool begin();
    void update();
    bool isConnected();

private:
    MotionController &_motion;
    EncoderAS5600 &_encoder;
    PowerAndThermalMonitor &_monitor;
    bool _is_connected;
    uint32_t _last_pub_ms;
};
