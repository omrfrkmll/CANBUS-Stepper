# ⚡ CANBUS-Stepper

<p align="center">
  <b>ESP32-S3 Based Closed-Loop CAN Bus Stepper Motor Driver Board</b>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/MCU-ESP32--S3-red?style=for-the-badge&logo=espressif&logoColor=white" alt="MCU ESP32-S3"/>
  <img src="https://img.shields.io/badge/Driver-TMC2209--LA-007ACC?style=for-the-badge" alt="Driver TMC2209"/>
  <img src="https://img.shields.io/badge/Bus-CAN%20Bus%20%2F%20CAN%20FD-orange?style=for-the-badge" alt="Bus CAN"/>
  <img src="https://img.shields.io/badge/Encoder-AS5600%20(12--Bit)-4BC51D?style=for-the-badge" alt="Encoder AS5600"/>
  <img src="https://img.shields.io/badge/EDA-KiCad%208.0-314CB6?style=for-the-badge&logo=kicad&logoColor=white" alt="EDA KiCad 8.0"/>
  <img src="https://img.shields.io/badge/Status-PCB%20Layout%20Complete-yellow?style=for-the-badge" alt="Status"/>
</p>

<p align="center">
  <a href="#-development-status">Development Status</a> •
  <a href="#-overview">Overview</a> •
  <a href="#-hardware-features--component-breakdown">Hardware Features</a> •
  <a href="#-pcb--hardware-preview">PCB Preview</a> •
  <a href="#-sponsorship--support">Sponsorship</a> •
  <a href="#-license--commercial-use">License</a>
</p>

---

## 🚧 Development Status

> [!WARNING]
> **Work In Progress (WIP)**: This project is currently in active **schematic design & PCB layout stage**. 
> The board schematics and PCB layout (45 unique component groups, 67 total parts fully mapped with LCSC C-codes) are complete. The hardware **has NOT yet been physically manufactured or field-tested**. Initial prototype batch production and physical hardware validation are pending.

---

## 📌 Overview

**CANBUS-Stepper** is a high-performance smart stepper motor driver board built around the **ESP32-S3** microcontroller. Designed specifically for 3D printer toolheads, robotic actuators, and CNC automation, this board integrates a native CAN Bus transceiver (TCAN1051V), silent stepper driver (TMC2209-LA), magnetic position encoder feedback (AS5600), power regulation (TI LMR33630 36V/3A DC-DC buck converter), and comprehensive circuit protection into a compact NEMA 17 footprint (42mm x 42mm).

---

## 🛠️ Hardware Features & Component Breakdown

### 🧠 Core System & Microcontroller
- **MCU**: **ESP32-S3-MINI-1** (Dual-core Xtensa LX7 @ 240MHz, 2.4GHz Wi-Fi + BLE 5, 8MB Flash, Native USB 2.0).

### ⚡ Motor Driver & Closed-Loop Feedback
- **Motor Driver**: **Trinamic TMC2209-LA** (SilentStepStick, StealthChop2, SpreadCycle, StallGuard4 sensorless homing, up to 2.0A RMS / 2.8A Peak drive current).
- **Position Sensor**: **AS5600-ASOT** 12-Bit Magnetic Rotary Encoder (I2C interface, $0.087^\circ$ resolution, $4096$ steps/rev) for high-precision closed-loop position monitoring.

### 🌐 Bus Communication & Power Management
- **CAN Bus Transceiver**: **TI TCAN1051VDRBRQ1** 5V CAN FD Transceiver with 3.3V $V_{IO}$ logic interface (VSON-8 3mm x 3mm).
- **Main Power Buck Regulator**: **TI LMR33630CRNXR** High-Efficiency Step-Down DC-DC Converter (3.8V–36V Input, 3A Output @ 2.1MHz switching frequency, VQFN-12 2mm x 3mm).
- **LDO Voltage Regulator**: **TI TLV75533PDRVR** 500mA 3.3V Low-Dropout Regulator (WSON-6 2mm x 2mm).

### 🛡️ Protection & Security Circuits
- **Reverse Polarity Protection**: **Alpha & Omega AON7407** -30V / -30A P-Channel MOSFET (DFN 3mm x 3mm).
- **CAN Bus ESD Protection**: **Nexperia PESD1CAN** TVS Diode Array.
- **Main Power TVS**: **SMAJ28A** 28V Transient Voltage Suppressor.
- **USB Protection**: **STMicroelectronics USBLC6-2P6** USB 2.0 High-Speed ESD protection array.
- **Overcurrent Protection**:
  - `1.5A 24V PPTC` Resettable fuse on main 24V DC input (1210 package).
  - `500mA 16V PPTC` Resettable fuse on USB VBUS power line.

### 🔌 Connectors & Interfaces
- **USB Interface**: USB Type-C 16-pin receptacle (Programming, firmware flashing & serial CDC debugging).
- **Power & Bus Input**: Molex Micro-Fit 3.0 2-Pin connector for 24V Main Power and CAN Bus Data lines.
- **Motor Output**: JST PH 4-Pin connector for 4-wire bipolar stepper motor phases.
- **Display Interface**: 30-Pin 0.5mm FPC Connector (SFV30R-2STBE1HLF) for OLED display panels.
- **Expansion Ports**:
  - Qwiic / I2C 4-Pin Expansion Connector (3.3V, GND, SDA, SCL).
  - AUX 4-Pin Expansion Header for auxiliary GPIOs / sensors.
  - NTC Thermistor Port (Dedicated input for motor/board temperature monitoring).
- **Switches & Termination**: On-board DIP/slide switches for 120Ω CAN Bus line termination resistor enable, boot mode, and reset.

---

## 🖼️ PCB & Hardware Preview

<p align="center">
  <img src="docs/assets/pcb_top.png?v=2" alt="CANBUS-Stepper PCB Top Side" width="48%"/>
  &nbsp;&nbsp;
  <img src="docs/assets/pcb_bottom.png?v=2" alt="CANBUS-Stepper PCB Bottom Side" width="48%"/>
  <br>
  <i>Figure 1: CANBUS-Stepper PCB Component Layout Preview — Top Side (Left) & Bottom Side (Right)</i>
</p>

---

## 💖 Sponsorship & Support

To bring this open-hardware project from design files to physical prototype boards, test hardware, and community adoption, **sponsorship and hardware support are warmly welcomed!**

If you are a **PCB manufacturing house, component supplier, or individual backer** interested in supporting:
- 🛠️ **Prototype PCB Fabrication & Assembly (PCBA)**
- 🧪 **Component Sourcing & Hardware Testing**
- ☕ **Developer Contributions & Project Funding**

Please feel free to reach out, open an issue, or sponsor through **GitHub Sponsors**. Your support directly accelerates physical prototyping and open-source hardware validation!

---

## 📁 Repository Structure

```
CANBUS-Stepper/
├── README.md                      # Main project documentation
├── LICENSE                        # Project License
├── .gitignore                     # Git ignore rules
├── docs/
│   └── assets/                    # Board renders, diagrams, and documentation assets
├── firmware/                      # ESP32-S3 Stepper Driver Firmware & Open SDK
│   └── README.md                  # Firmware architecture & CAN bus protocol roadmap
└── step/                          # 3D CAD STEP Files & Enclosure Integration Models
    └── README.md                  # Exported overall PCB 3D STEP model for mechanical integration
```

---

## 📜 Firmware & SDK Roadmap

Firmware and SDK development for the ESP32-S3 is structured under `firmware/`. Key roadmap milestones include:
- **TWAI CAN Bus Protocol**: Standardized command set for motor movement, status telemetry, and configuration.
- **Closed-Loop Control**: AS5600 encoder feedback loop for stall detection and precise step regulation.
- **Open-Source SDK & API**: C++ / Python libraries and robotic actuator control guides.

---

## ⚖️ Project Architecture & Commercial Licensing

This project follows a **Hybrid Open-Software & Proprietary-Hardware** model:

- 💻 **Firmware, SDK & Software (Open Source)**: Open for community contributions, Klipper/Marlin integration, and custom robotics control.
- 🛡️ **Hardware Design & Schematics (Proprietary IP)**: The PCB layout and Gerber design files are proprietary. 

### 🏢 Manufacturing & Commercial Partnerships
For prototype support, custom batch manufacturing, or exclusive regional retail distribution partnerships, please contact the repository owner.
