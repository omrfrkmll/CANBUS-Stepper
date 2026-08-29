#include <Arduino.h>
#include "config.h"
#include "modes.h"
#include "encoder_as5600.h"
#include "ntc_sensor.h"
#include "tmc2209_handler.h"
#include "motion_controller.h"
#include "can_protocol.h"
#include "microros_handler.h"

// --- Global Hardware Objects ---
EncoderAS5600 encoder;
PowerAndThermalMonitor monitor;
TMC2209Handler driver;
MotionController motion(driver, encoder);
CANBusProtocol can_bus(motion, encoder, monitor, driver);
MicroROSHandler ros_node(motion, encoder, monitor);

// --- Task & State Variables ---
TaskHandle_t motionTaskHandle = NULL;
uint32_t last_heartbeat_ms = 0;
uint32_t last_diag_log_ms = 0;
bool led_state = false;

// --- High Priority Motion & PID Control Task (Core 1) ---
void motionControlTask(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(1); // 1 ms = 1000 Hz loop

    for (;;) {
        motion.update();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

// --- Interactive Serial Console Parser ---
void handleSerialCommands() {
    if (Serial.available() > 0) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        if (cmd.startsWith("P ")) {
            float pos = cmd.substring(2).toFloat();
            motion.setTargetPosition(pos);
            motion.setControlMode(ControlMode::CLOSED_LOOP_POSITION);
            Serial.printf("[CMD] Target Position set to: %.2f deg\n", pos);
        } else if (cmd.startsWith("V ")) {
            float vel = cmd.substring(2).toFloat();
            motion.setTargetVelocity(vel);
            motion.setControlMode(ControlMode::CLOSED_LOOP_VELOCITY);
            Serial.printf("[CMD] Target Velocity set to: %.2f deg/s\n", vel);
        } else if (cmd.equalsIgnoreCase("EN")) {
            motion.enable(true);
            Serial.println("[CMD] Motor Enabled (Closed Loop)");
        } else if (cmd.equalsIgnoreCase("DIS")) {
            motion.enable(false);
            Serial.println("[CMD] Motor Disabled");
        } else if (cmd.equalsIgnoreCase("ZERO")) {
            encoder.setZeroPosition();
            motion.setTargetPosition(0.0f);
            Serial.println("[CMD] Zero position calibrated");
        } else if (cmd.startsWith("CUR ")) {
            uint16_t ma = (uint16_t)cmd.substring(4).toInt();
            driver.setCurrent(ma);
            Serial.printf("[CMD] Motor Current set to: %d mA\n", ma);
        } else if (cmd.startsWith("PID ")) {
            // PID <kp> <ki> <kd>
            float kp, ki, kd;
            if (sscanf(cmd.c_str(), "PID %f %f %f", &kp, &ki, &kd) == 3) {
                motion.setPIDGains(kp, ki, kd);
                Serial.printf("[CMD] PID Gains set: Kp=%.2f, Ki=%.3f, Kd=%.3f\n", kp, ki, kd);
            }
        } else if (cmd.equalsIgnoreCase("STATUS") || cmd.equalsIgnoreCase("?")) {
            Serial.println("\n========== CANBUS-Stepper v1.1 Status ==========");
            Serial.printf("Encoder Angle: %.2f deg (Raw: %d, Turns: %d)\n", 
                          encoder.getAngleDegrees(), encoder.getRawAngle(), encoder.getMultiTurnAngle() / 4096);
            Serial.printf("Target Position: %.2f deg | Error: %.2f deg\n", 
                          motion.getTargetPosition(), motion.getPositionError());
            Serial.printf("Velocity: %.2f deg/s\n", encoder.getVelocityDegPerSec());
            Serial.printf("VBUS Voltage: %.2f V | Temperature: %.1f C\n", 
                          monitor.getVbusVoltage(), monitor.getTemperatureC());
            Serial.printf("CAN Bus: %s (RX: %d, TX: %d)\n", 
                          can_bus.isBusHealthy() ? "HEALTHY" : "ERROR/OFFLINE", can_bus.getRxCount(), can_bus.getTxCount());
            AS5600Status s = encoder.getStatus();
            Serial.printf("Magnet: %s (Gain AGC: %d, Magnitude: %d)\n", 
                          s.magnet_detected ? "DETECTED" : "MISSING", s.gain_agc, s.magnitude);
            Serial.println("================================================\n");
        }
    }
}

void setup() {
    // 1. Initialize USB CDC Serial
    Serial.begin(115200);
    delay(500);

    Serial.println("\n=======================================================");
    Serial.println("⚡ CANBUS-Stepper v1.1 Smart Closed-Loop Controller");
    Serial.println("   ESP32-S3 + TMC2209 + AS5600 + TCAN1051V");
    Serial.println("=======================================================");

    // 2. Setup User GPIOs
    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    pinMode(PIN_SW1, INPUT_PULLUP);
    pinMode(PIN_SW2, INPUT_PULLUP);
    pinMode(PIN_AUX1, INPUT_PULLUP);
    pinMode(PIN_AUX2, INPUT_PULLUP);

    digitalWrite(PIN_LED1, HIGH);
    digitalWrite(PIN_LED2, LOW);

    // 3. Initialize ADC Power & Temperature Monitor
    monitor.begin();
    Serial.printf("[INIT] VBUS Voltage: %.2fV | Board Temp: %.1fC\n", monitor.getVbusVoltage(), monitor.getTemperatureC());

    // 4. Initialize AS5600 Magnetic Rotary Encoder
    bool enc_ok = encoder.begin();
    if (enc_ok) {
        Serial.printf("[INIT] AS5600 Encoder: OK (Angle: %.2f deg)\n", encoder.getAngleDegrees());
    } else {
        Serial.println("[INIT] AS5600 Encoder: WARNING - Magnet not detected!");
    }

    // 5. Initialize TMC2209 Stepper Driver
    bool tmc_ok = driver.begin();
    if (tmc_ok) {
        Serial.println("[INIT] TMC2209 Stepper Driver: OK (UART Connected)");
    } else {
        Serial.println("[INIT] TMC2209 Stepper Driver: UART not responding (will run in standalone STEP/DIR mode)");
    }

    // 6. Initialize Motion Controller
    motion.begin();
    Serial.println("[INIT] Closed-Loop Motion Controller Initialized");

    // 7. Initialize TWAI CAN Bus Protocol
    bool can_ok = can_bus.begin(DEFAULT_CAN_BAUD, DEFAULT_CAN_NODE_ID);
    if (can_ok) {
        Serial.printf("[INIT] TCAN1051V CAN Bus: RUNNING @ %d bps (Node ID: 0x%X)\n", DEFAULT_CAN_BAUD, DEFAULT_CAN_NODE_ID);
    } else {
        Serial.println("[INIT] TCAN1051V CAN Bus: Failed to start TWAI driver");
    }

    // 8. Launch High-Priority Motion Control Task on Core 1
    xTaskCreatePinnedToCore(
        motionControlTask,
        "MotionTask",
        4096,
        NULL,
        configMAX_PRIORITIES - 1, // Highest priority
        &motionTaskHandle,
        1 // Core 1
    );

    Serial.println("[INIT] System Ready! Type '?' or 'STATUS' for telemetry.\n");
}

void loop() {
    // 1. Update CAN Bus Protocol & process incoming frames
    can_bus.update();

    // 2. Update ADC monitoring (VBUS & Temperature)
    monitor.update();

    // 3. Safety Thermal & Undervoltage Interlocks
    if (monitor.isOverheated(90.0f)) {
        motion.emergencyStop();
        digitalWrite(PIN_LED2, HIGH);
        Serial.println("[ALERT] EMERGENCY STOP: Overtemperature detected!");
    }

    // 4. Handle Hardware User Buttons
    if (digitalRead(PIN_SW2) == LOW) { // E-Stop button
        motion.emergencyStop();
        delay(200);
    }
    if (digitalRead(PIN_SW1) == LOW) { // Zero calibration button
        encoder.setZeroPosition();
        motion.setTargetPosition(0.0f);
        delay(200);
    }

    // 5. Handle Serial CLI input
    handleSerialCommands();

    // 6. Heartbeat LED blink (1 Hz)
    uint32_t now_ms = millis();
    if (now_ms - last_heartbeat_ms >= 500) {
        last_heartbeat_ms = now_ms;
        led_state = !led_state;
        digitalWrite(PIN_LED1, led_state ? HIGH : LOW);

        // Flash CAN LED if bus has activity
        if (can_bus.getRxCount() > 0) {
            digitalWrite(PIN_LED2, (can_bus.getRxCount() % 2 == 0) ? HIGH : LOW);
        }
    }

    vTaskDelay(pdMS_TO_TICKS(5));
}
