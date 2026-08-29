#pragma once

#include <Arduino.h>

// =============================================================================
// CANBUS-Stepper v1.0 Hardware Configuration & Pinout Map
// =============================================================================

// --- TMC2209-LA Stepper Driver Pins ---
#define PIN_TMC_STEP        33  // Step pulse input (IO33 / Pad 28)
#define PIN_TMC_DIR         34  // Direction control (IO34 / Pad 29)
#define PIN_TMC_EN          48  // Driver Enable (Active LOW) (IO48 / Pad 30)
#define PIN_TMC_TX          35  // ESP32 TX -> TMC2209 RX (UART) (IO35 / Pad 31)
#define PIN_TMC_RX          47  // ESP32 RX <- TMC2209 TX (UART) (IO47 / Pad 27)
#define PIN_TMC_DIAG        21  // StallGuard / Fault interrupt (IO21 / Pad 25)
#define PIN_TMC_INDEX       26  // Microstep index output (IO26 / Pad 26)

#define TMC_SERIAL_PORT     Serial1
#define TMC_BAUDRATE        115200
#define TMC_R_SENSE         0.11f  // 0.11 Ohm sense resistors (R4, R6 in schematic)
#define TMC_DRIVER_ADDRESS  0b00   // MS1=0, MS2=0 address

// --- AMS AS5600 12-Bit Magnetic Rotary Encoder ---
#define PIN_I2C_SDA         16  // Hardware I2C SDA (IO16 / Pad 20)
#define PIN_I2C_SCL         14  // Hardware I2C SCL (IO14 / Pad 18)
#define I2C_FREQUENCY       400000 // 400 kHz Fast-mode I2C
#define AS5600_I2C_ADDR     0x36

#define ENCODER_RESOLUTION  4096   // 12-bit (0 - 4095)
#define DEG_PER_COUNT       (360.0f / 4096.0f) // 0.08789 degrees per count

// --- CAN Bus / TWAI Transceiver (TI TCAN1051V) ---
#define PIN_CAN_TX          38  // ESP32 TWAI TX (IO38 / Pad 34)
#define PIN_CAN_RX          37  // ESP32 TWAI RX (IO37 / Pad 33)
#define PIN_CAN_STB         39  // Standby pin (LOW = Normal Mode, HIGH = Standby) (IO39 / Pad 35)
#define DEFAULT_CAN_BAUD    500000 // 500 kbps default CAN baudrate (supports up to 1Mbps)
#define DEFAULT_CAN_NODE_ID 0x120  // Default Node ID for actuator

// --- Analog Sensing (ADC1) ---
#define PIN_VBUS_SENSE      2   // 24V/VBUS Voltage divider input (ADC1_CH1 / IO2 / Pad 6)
#define PIN_NTC_SENSE       3   // On-board / Motor NTC Thermistor (ADC1_CH2 / IO3 / Pad 7)

#define VBUS_R_TOP          100000.0f // 100k Ohm top divider (R20)
#define VBUS_R_BOTTOM       10000.0f  // 10k Ohm bottom divider (R14)
#define VBUS_DIVIDER_RATIO  ((VBUS_R_TOP + VBUS_R_BOTTOM) / VBUS_R_BOTTOM) // 11.0x

#define NTC_R_SERIES        10000.0f  // 10k series pull-up resistor (R3)
#define NTC_NOMINAL_R       100000.0f // 100k nominal resistance at 25C (NCU18WF104)
#define NTC_NOMINAL_TEMP    25.0f     // 25 °C
#define NTC_B_COEFFICIENT   4250.0f   // Beta coefficient (B25/50)

// --- User Controls & Status Indicators ---
#define PIN_LED1            1   // Status LED 1 (Heartbeat / Activity) (IO1 / Pad 5)
#define PIN_LED2            45  // Status LED 2 (CAN Activity / Warning) (IO45 / Pad 41)
#define PIN_SW1             6   // Tactile Switch 1 (User / Mode Select) (IO6 / Pad 10)
#define PIN_SW2             36  // Tactile Switch 2 (Calibrate / E-Stop) (IO36 / Pad 32)

// --- Expansion & Auxiliary GPIOs ---
#define PIN_AUX1            4   // AUX Header GPIO 1 / Endstop 1 (IO4 / Pad 8)
#define PIN_AUX2            5   // AUX Header GPIO 2 / Endstop 2 (IO5 / Pad 9)

// --- Motion & PID Default Parameters ---
#define DEFAULT_MICROSTEPS  16
#define STEPS_PER_REV_NEMA  (200 * DEFAULT_MICROSTEPS) // 3200 steps/rev for 1.8 deg motor
#define DEFAULT_MAX_VELOCITY 50000.0f // steps/s
#define DEFAULT_ACCEL       100000.0f // steps/s^2

#define PID_KP              2.5f
#define PID_KI              0.05f
#define PID_KD              0.12f
#define PID_LOOP_HZ         1000  // 1 kHz Closed-Loop Control Frequency
