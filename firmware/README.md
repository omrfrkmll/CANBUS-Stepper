# 🧠 CANBUS-Stepper Firmware & Control SDK

This directory contains the firmware architecture, CAN bus communication protocol, and control SDK for the **ESP32-S3** microcontroller on the **CANBUS-Stepper** closed-loop driver board.

---

## 🏗️ Architecture & Core Components

- **Target MCU**: ESP32-S3-MINI-1-N4R2 (Dual-core Xtensa LX7 @ 240MHz, 4MB Flash, 2MB PSRAM)
- **Framework**: ESP-IDF v5.x / Arduino ESP32 Core
- **Motor Control**: Trinamic TMC2209-LA via UART (microstepping, StealthChop2, StallGuard4 sensorless homing) + Step/Dir pulse generation.
- **Closed-Loop Feedback**: AS5600 12-Bit Magnetic Rotary Encoder via fast hardware I2C (400kHz), running high-frequency angle reading & real-time PID position error correction.
- **Communication Stack**: ESP32-S3 Native TWAI (Two-Wire Automotive Interface) CAN Bus driver via TI TCAN1051V transceiver.

---

## 🗺️ Firmware Development Roadmap

- [ ] **Phase 1: Board Bring-up & Diagnostics**
  - [x] Hardware pinout mapping & peripheral definitions.
  - [ ] Power rail validation, I2C bus scanner (AS5600 detection & magnet field strength check).
  - [ ] TMC2209 UART communication & register read/write tests.
  - [ ] Native USB CDC serial console & web flasher integration.

- [ ] **Phase 2: Motor Motion & Closed-Loop Control**
  - [ ] High-frequency RMT / Timer-based step pulse generation with acceleration curves.
  - [ ] Closed-loop PID position regulator using AS5600 feedback.
  - [ ] Stall detection, missed step recovery, and overload protection.

- [ ] **Phase 3: Bus Protocols & Ecosystem Integration**
  - [ ] Standardized CAN Bus binary packet protocol (Position, Velocity, Current, Status telemetry).
  - [ ] **Klipper CAN Bus Toolhead / Stepper Node** protocol compatibility.
  - [ ] **Micro-ROS / ROS2** actuator node support for robotic arms and AGVs.
  - [ ] Open-source Python & C++ client SDKs with GUI motor tuning tools.

---

## 🤝 Contributing
Firmware contributions, issue reports, and feature requests are warmly welcomed! Please refer to the main repository [LICENSE](../LICENSE) (MIT License).
