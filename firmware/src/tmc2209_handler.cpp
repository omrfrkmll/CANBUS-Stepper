#include "tmc2209_handler.h"

TMC2209Handler::TMC2209Handler()
    : _driver(&TMC_SERIAL_PORT, TMC_R_SENSE, TMC_DRIVER_ADDRESS),
      _is_enabled(false),
      _current_ma(800),
      _microsteps(DEFAULT_MICROSTEPS) {}

bool TMC2209Handler::begin() {
    pinMode(PIN_TMC_EN, OUTPUT);
    pinMode(PIN_TMC_STEP, OUTPUT);
    pinMode(PIN_TMC_DIR, OUTPUT);
    pinMode(PIN_TMC_DIAG, INPUT);
    pinMode(PIN_TMC_INDEX, INPUT);

    digitalWrite(PIN_TMC_EN, HIGH); // Disabled by default
    digitalWrite(PIN_TMC_STEP, LOW);
    digitalWrite(PIN_TMC_DIR, LOW);

    // Initialize UART to TMC2209
    TMC_SERIAL_PORT.begin(TMC_BAUDRATE, SERIAL_8N1, PIN_TMC_RX, PIN_TMC_TX);

    delay(20);
    _driver.begin();

    // Verify communication by reading version
    uint8_t version = _driver.version();
    bool comm_ok = (version == 0x21 || version == 0x20);

    // Basic driver configuration
    _driver.toff(4);                 // Enable driver in software
    _driver.blank_time(24);
    _driver.rms_current(_current_ma); // Default current in mA
    _driver.microsteps(_microsteps);
    _driver.pwm_autoscale(true);     // StealthChop auto-tuning
    _driver.en_spreadCycle(false);   // StealthChop default (silent)
    _driver.TCOOLTHRS(0xFFFFF);      // CoolStep / StallGuard threshold
    _driver.semin(5);
    _driver.semax(2);
    _driver.sedn(0b01);
    _driver.SGTHRS(100);             // StallGuard sensitivity (0 - 255)

    return comm_ok;
}

void TMC2209Handler::enable(bool state) {
    _is_enabled = state;
    digitalWrite(PIN_TMC_EN, state ? LOW : HIGH); // Active LOW
    _driver.toff(state ? 4 : 0);
}

void TMC2209Handler::setCurrent(uint16_t current_ma) {
    _current_ma = current_ma;
    _driver.rms_current(current_ma);
}

void TMC2209Handler::setMicrosteps(uint16_t microsteps) {
    _microsteps = microsteps;
    _driver.microsteps(microsteps);
}

void TMC2209Handler::setStealthChop(bool enable) {
    _driver.en_spreadCycle(!enable);
}

void TMC2209Handler::setStallGuardThreshold(uint8_t threshold) {
    _driver.SGTHRS(threshold);
}

TMCStatus TMC2209Handler::readStatus() {
    TMCStatus status = {0};
    uint32_t drv_status = _driver.DRV_STATUS();
    
    status.is_communicating = (_driver.version() != 0);
    status.overtemperature = _driver.ot();
    status.overtemperature_prewarning = _driver.otpw();
    status.short_to_ground_a = _driver.s2ga();
    status.short_to_ground_b = _driver.s2gb();
    status.open_load_a = _driver.ola();
    status.open_load_b = _driver.olb();
    status.stand_still = _driver.stst();
    status.sg_result = _driver.SG_RESULT();

    return status;
}

bool TMC2209Handler::isStalled() {
    return digitalRead(PIN_TMC_DIAG) == HIGH;
}
