#include "ntc_sensor.h"
#include <math.h>

PowerAndThermalMonitor::PowerAndThermalMonitor()
    : _temperature_c(25.0f),
      _vbus_voltage(24.0f),
      _last_sample_ms(0) {}

void PowerAndThermalMonitor::begin() {
    pinMode(PIN_VBUS_SENSE, INPUT);
    pinMode(PIN_NTC_SENSE, INPUT);
    analogReadResolution(12); // 12-bit ADC (0 - 4095)
    update();
}

void PowerAndThermalMonitor::update() {
    uint32_t now_ms = millis();
    if (now_ms - _last_sample_ms < 50) { // Sample every 50ms
        return;
    }
    _last_sample_ms = now_ms;

    // --- VBUS Voltage Measurement ---
    uint32_t raw_vbus = analogRead(PIN_VBUS_SENSE);
    float v_adc_vbus = ((float)raw_vbus / 4095.0f) * 3.3f;
    _vbus_voltage = (_vbus_voltage * 0.9f) + (v_adc_vbus * VBUS_DIVIDER_RATIO * 0.1f);

    // --- NTC Temperature (Steinhart-Hart Equation / Beta Formula) ---
    uint32_t raw_ntc = analogRead(PIN_NTC_SENSE);
    if (raw_ntc > 0 && raw_ntc < 4090) {
        float v_ntc = ((float)raw_ntc / 4095.0f) * 3.3f;
        // Voltage divider: R_NTC = R_SERIES * (V_NTC / (3.3 - V_NTC))
        float r_ntc = NTC_R_SERIES * (v_ntc / (3.3f - v_ntc));

        float steinhart;
        steinhart = r_ntc / NTC_NOMINAL_R;               // (R/Ro)
        steinhart = log(steinhart);                      // ln(R/Ro)
        steinhart /= NTC_B_COEFFICIENT;                  // 1/B * ln(R/Ro)
        steinhart += 1.0f / (NTC_NOMINAL_TEMP + 273.15f); // + (1/To)
        steinhart = 1.0f / steinhart;                    // Invert
        float temp_c = steinhart - 273.15f;              // Convert Kelvin to Celsius

        _temperature_c = (_temperature_c * 0.9f) + (temp_c * 0.1f);
    }
}

float PowerAndThermalMonitor::getTemperatureC() {
    return _temperature_c;
}

float PowerAndThermalMonitor::getVbusVoltage() {
    return _vbus_voltage;
}

bool PowerAndThermalMonitor::isOverheated(float threshold_c) {
    return _temperature_c > threshold_c;
}

bool PowerAndThermalMonitor::isUndervoltage(float threshold_v) {
    return _vbus_voltage < threshold_v;
}
