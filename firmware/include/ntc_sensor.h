#pragma once

#include <Arduino.h>
#include "config.h"

class PowerAndThermalMonitor {
public:
    PowerAndThermalMonitor();

    void begin();
    void update();

    float getTemperatureC();
    float getVbusVoltage();
    bool isOverheated(float threshold_c = 85.0f);
    bool isUndervoltage(float threshold_v = 10.0f);

private:
    float _temperature_c;
    float _vbus_voltage;
    uint32_t _last_sample_ms;
};
