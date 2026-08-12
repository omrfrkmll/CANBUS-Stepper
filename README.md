# ⚡ CANBUS-Stepper

<p align="center">
  <b>ESP32-S3 Based Closed-Loop CAN Bus Stepper Motor Driver Board</b>
</p>

<p align="center">
  <a href="#-development-status">Development Status</a> •
  <a href="#-overview">Overview</a> •
  <a href="#-hardware-features">Hardware Features</a> •
  <a href="#-pcb--hardware-preview">PCB Preview</a> •
  <a href="#-sponsorship--support">Sponsorship</a> •
  <a href="#-license--commercial-use">License</a>
</p>

---

## 🚧 Development Status

> [!WARNING]
> **Work In Progress (WIP)**: This project is currently in active **schematic design & PCB layout stage**. 
> The board **has NOT yet been physically manufactured or field-tested**. The schematics and component placements may undergo further design revisions before initial prototype batch production and physical hardware validation.

---

## 📌 Overview

**CANBUS-Stepper** is an open-source, high-performance smart stepper motor driver board built around the **ESP32-S3** microcontroller. Designed specifically for 3D printer toolheads, robotic actuators, and CNC automation, this board integrates a native CAN Bus transceiver (TCAN1051V), magnetic position encoder feedback (AS5600), power regulation (TI LMR33630 36V/3A buck converter), and stepper motor driving into a compact NEMA 17 footprint.

---

## 🛠️ Hardware Features

- **Microcontroller**: ESP32-S3 (Dual-core Xtensa LX7 @ 240MHz, Native USB 2.0, TWAI / CAN Bus support).
- **CAN Bus Transceiver**: TCAN1051VDRBRQ1 5V CAN FD Transceiver with 3.3V $V_{IO}$ logic interface (VSON-8 3mm x 3mm).
- **Power Management**: TI LMR33630CRNXR High-efficiency 3.8V–36V / 3A step-down buck converter (VQFN-12 2mm x 3mm).
- **Reverse Polarity Protection**: Alpha & Omega AON7407 -30V/-30A P-Channel MOSFET (DFN 3mm x 3mm).
- **Position Feedback**: AS5600 Magnetic Rotary Encoder (I2C interface) for accurate absolute angle sensing and closed-loop control.
- **Connectivity & I/O**:
  - USB Type-C receptacle for programming, flashing, and serial debugging.
  - Molex Micro-Fit 3.0 & JST connectors for CAN Bus power/data and motor phase outputs.
  - SPDT Slide switches for boot mode and power control.

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
- **Open-Source SDK & API**: C++ / Python kütüphaneleri ve robot kolu (robotic actuator) kontrol rehberleri.

---

## ⚖️ Project Architecture & Commercial Licensing

This project follows a **Hybrid Open-Software & Proprietary-Hardware** model:

- 💻 **Firmware, SDK & Software (Open Source)**: Open for community contributions, Klipper/Marlin integration, and custom robotics control.
- 🛡️ **Hardware Design & Schematics (Proprietary IP)**: The PCB layout and Gerber design files are proprietary. 

### 🏢 Manufacturing & Commercial Partnerships
For prototype support, custom batch manufacturing, or exclusive regional retail distribution partnerships, please contact the repository owner.

