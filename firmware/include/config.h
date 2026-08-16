#pragma once

#include <Arduino.h>

// =============================================================================
// CANBUS-Stepper v2.1 Hardware Configuration & Pinout Map
// =============================================================================

// --- TMC2209-LA Stepper Driver Pins ---
#define PIN_TMC_STEP        5   // Step pulse input
#define PIN_TMC_DIR         6   // Direction control
#define PIN_TMC_EN          21  // Driver Enable (Active LOW)
#define PIN_TMC_TX          17  // ESP32 TX -> TMC2209 RX (UART)
#define PIN_TMC_RX          18  // ESP32 RX <- TMC2209 TX (UART)
#define PIN_TMC_DIAG        16  // StallGuard / Fault interrupt
#define PIN_TMC_INDEX       11  // Microstep index output

#define TMC_SERIAL_PORT     Serial1
#define TMC_BAUDRATE        115200
#define TMC_R_SENSE         0.11f  // 0.11 Ohm sense resistors (R4, R6 in schematic)
#define TMC_DRIVER_ADDRESS  0b00   // MS1=0, MS2=0 address

// --- AMS AS5600 12-Bit Magnetic Rotary Encoder ---
#define PIN_I2C_SDA         8   // Hardware I2C SDA
#define PIN_I2C_SCL         9   // Hardware I2C SCL
#define I2C_FREQUENCY       400000 // 400 kHz Fast-mode I2C
#define AS5600_I2C_ADDR     0x36

#define ENCODER_RESOLUTION  4096   // 12-bit (0 - 4095)
#define DEG_PER_COUNT       (360.0f / 4096.0f) // 0.08789 degrees per count

// --- CAN Bus / TWAI Transceiver (TI TCAN1051V) ---
#define PIN_CAN_TX          48  // ESP32 TWAI TX
#define PIN_CAN_RX          47  // ESP32 TWAI RX
#define PIN_CAN_STB         37  // Standby pin (LOW = Normal Mode, HIGH = Standby)
#define DEFAULT_CAN_BAUD    500000 // 500 kbps default CAN baudrate (supports up to 1Mbps)
#define DEFAULT_CAN_NODE_ID 0x120  // Default Node ID for actuator

// --- Analog Sensing (ADC1) ---
#define PIN_VBUS_SENSE      4   // 24V/VBUS Voltage divider input (ADC1_CH3)
#define PIN_NTC_SENSE       7   // On-board / Motor NTC Thermistor (ADC1_CH6)

#define VBUS_R_TOP          100000.0f // 100k Ohm top divider (R15)
#define VBUS_R_BOTTOM       10000.0f  // 10k Ohm bottom divider (R24)
#define VBUS_DIVIDER_RATIO  ((VBUS_R_TOP + VBUS_R_BOTTOM) / VBUS_R_BOTTOM) // 11.0x

#define NTC_R_SERIES        10000.0f  // 10k series pull-up resistor (R22)
#define NTC_NOMINAL_R       100000.0f // 100k nominal resistance at 25C (NCU18WF104)
#define NTC_NOMINAL_TEMP    25.0f     // 25 °C
#define NTC_B_COEFFICIENT   4250.0f   // Beta coefficient (B25/50)

// --- User Controls & Status Indicators ---
#define PIN_LED1            10  // Status LED 1 (Heartbeat / Activity)
#define PIN_LED2            12  // Status LED 2 (CAN Activity / Warning)
#define PIN_SW1             35  // Tactile Switch 1 (User / Mode Select)
#define PIN_SW2             36  // Tactile Switch 2 (Calibrate / E-Stop)

// --- Expansion & Auxiliary GPIOs ---
#define PIN_AUX1            14  // AUX Header GPIO 1 / Endstop 1
#define PIN_AUX2            13  // AUX Header GPIO 2 / Endstop 2

// --- Motion & PID Default Parameters ---
#define DEFAULT_MICROSTEPS  16
#define STEPS_PER_REV_NEMA  (200 * DEFAULT_MICROSTEPS) // 3200 steps/rev for 1.8 deg motor
#define DEFAULT_MAX_VELOCITY 50000.0f // steps/s
#define DEFAULT_ACCEL       100000.0f // steps/s^2

#define PID_KP              2.5f
#define PID_KI              0.05f
#define PID_KD              0.12f
#define PID_LOOP_HZ         1000  // 1 kHz Closed-Loop Control Frequency
