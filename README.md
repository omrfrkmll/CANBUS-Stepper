# ⚡ CANBUS-Stepper

<p align="center">
  <b>Open-Source ESP32-S3 Closed-Loop CAN Bus Stepper Motor Driver Board</b>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Hardware-Open%20Source%20(CC%20BY--SA%204.0)-blue?style=for-the-badge&logo=open-source-initiative&logoColor=white" alt="Open Hardware"/>
  <img src="https://img.shields.io/badge/Firmware-MIT%20License-green?style=for-the-badge" alt="Firmware License"/>
  <img src="https://img.shields.io/badge/MCU-ESP32--S3-red?style=for-the-badge&logo=espressif&logoColor=white" alt="MCU ESP32-S3"/>
  <img src="https://img.shields.io/badge/Driver-TMC2209--LA-007ACC?style=for-the-badge" alt="Driver TMC2209"/>
  <img src="https://img.shields.io/badge/Bus-CAN%20Bus%20%2F%20CAN%20FD-orange?style=for-the-badge" alt="Bus CAN"/>
  <img src="https://img.shields.io/badge/Encoder-AS5600%20(12--Bit)-4BC51D?style=for-the-badge" alt="Encoder AS5600"/>
  <img src="https://img.shields.io/badge/EDA-KiCad%208.0-314CB6?style=for-the-badge&logo=kicad&logoColor=white" alt="EDA KiCad 8.0"/>
</p>

<p align="center">
  <a href="#-development-status">Development Status</a> •
  <a href="#-overview">Overview</a> •
  <a href="#-hardware-features--component-breakdown">Hardware Features</a> •
  <a href="#-pcb--hardware-preview">PCB Preview</a> •
  <a href="#-pcb-fabrication--production-files">Fabrication</a> •
  <a href="#-official-hardware--custom-engineering">Official Hardware & Services</a> •
  <a href="#-license">License</a>
</p>

---

## 🚧 Development Status

> [!WARNING]
> **Work In Progress (WIP)**: This project is in active **hardware prototyping and firmware bring-up stage**. 
> The board schematics, PCB layout (4-layer stackup, 45 unique component groups, 67 total parts fully mapped with LCSC C-codes), and mechanical STEP models are complete. Physical prototype fabrication and hardware validation are underway.

---

## 📌 Overview

**CANBUS-Stepper** is an open-source, high-performance smart stepper motor driver board built around the **ESP32-S3** microcontroller. Designed for 3D printer toolheads, robotic actuators, AGVs, and industrial automation, this board integrates:
- **Native CAN Bus Transceiver** (TI TCAN1051V)
- **Silent Stepper Driver** (Trinamic TMC2209-LA with StealthChop2 & StallGuard4)
- **Magnetic Position Encoder** (AS5600 12-Bit On-Axis Hall Sensor)
- **High-Efficiency Step-Down Buck Regulator** (TI LMR33630 36V/3A DC-DC)
- **Comprehensive Circuit Protection** (Reverse polarity, overcurrent PPTC, TVS & ESD diodes)
- **Ultra-compact NEMA 17 Footprint** (42mm x 42mm, direct motor rear mount)

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

## 📁 Repository Structure

```
CANBUS-Stepper/
├── README.md                      # Main project documentation
├── LICENSE                        # Open Source Hardware (CC BY-SA 4.0) & Software (MIT) License
├── .gitignore                     # Git ignore rules for KiCad & build artifacts
├── docs/
│   └── assets/                    # Board renders, 3D previews, and diagrams
├── pcb/                           # Complete KiCad 8.0 Design Source Files
│   ├── canbus-stepper.kicad_sch   # Full schematic design
│   ├── canbus-stepper.kicad_pcb   # 4-Layer PCB layout & routing
│   ├── canbus-stepper.kicad_pro   # KiCad project file
│   ├── CANBUS-Stepper.kicad_sym   # Project-specific schematic symbol library
│   ├── CANBUS-Stepper.pretty/     # Custom footprint library
│   ├── CANBUS-Stepper.3dshapes/   # 3D STEP / WRL component models
│   ├── bom/                       # Interactive HTML BOM (ibom.html) & CSV BOM
│   └── production/                # Ready-to-order Gerbers, pick-and-place & JLCPCB files
├── firmware/                      # ESP32-S3 Stepper Driver Firmware & Open SDK
│   └── README.md                  # Firmware architecture & CAN bus protocol roadmap
└── step/                          # Exported 3D CAD STEP Files for mechanical integration
    └── README.md                  # 3D mechanical enclosure & mounting models
```

---

## 🏭 PCB Fabrication & Production Files

The hardware is designed for immediate fabrication and SMT assembly with **JLCPCB**, **PCBWay**, or standard PCBA manufacturers:
- **Interactive BOM**: Open `pcb/bom/ibom.html` in any browser to interactively inspect and locate all 67 parts on the board.
- **Production Package**: Located in `pcb/production/`, containing Gerbers, drill files, IPC netlists, pick-and-place positions (`positions.csv`), and pre-mapped LCSC parts list (`bom.csv`).
- **3D Mechanical Model**: The full PCB assembly is exported under `step/` for 3D printing custom mounts and direct integration onto NEMA 17 motors.

---

## 🛒 Official Hardware, Kits & Custom Engineering

While the hardware and firmware are 100% open source, you can support the project and save time by ordering **Official Pre-Assembled & Tested Boards**:

### 📦 Why Buy Official Tested Hardware?
- 🛠️ **Plug-and-Play**: Fully assembled with precision SMT, flashed with verified bootloader & firmware, and 100% factory-tested.
- 🧲 **Complete Turnkey Kits**: Bundled with high-temperature NEMA 17 stepper motors, diametrically magnetized diametral magnets, mounting brackets, and pre-crimped CAN Bus wiring harnesses.
- 🤝 **Support Open Development**: Direct sales fund ongoing firmware development, Klipper modules, ROS2 integration, and new open-source hardware features.

### 🏢 B2B Custom Robotics & Enterprise Solutions
Need a modified version (e.g., higher voltage 48V, multi-axis integration, RS485, custom form factors, or specialized CAN FD / DroneCAN firmware)?
- **Custom Hardware Design & Prototyping**
- **Industrial Firmware & ROS2 / Micro-ROS Drivers**
- **Batch Manufacturing & Deployment Support**

👉 *For pre-orders, volume inquiries, or custom engineering consulting, please reach out via GitHub Issues or contact the project maintainer.*

---

## 📜 Firmware & SDK Roadmap

Firmware and SDK development for the ESP32-S3 is structured under `firmware/`. Key roadmap milestones include:
- **TWAI CAN Bus Protocol**: Standardized command set for motor velocity/position control, status telemetry, and configuration.
- **Closed-Loop Regulation**: Real-time AS5600 encoder feedback loop for position correction and stall recovery.
- **Klipper & Ecosystem Integration**: Native Klipper CAN bus toolhead/stepper node support.
- **Open-Source SDK & API**: C++ / Python libraries and ROS2 robotic actuator nodes.

---

## ⚖️ License

This project is released under open-source licenses:

- 🛡️ **Hardware (Schematics, PCB, CAD & 3D Models)**: Licensed under [Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)](https://creativecommons.org/licenses/by-sa/4.0/).
- 💻 **Firmware, SDK & Software**: Licensed under the [MIT License](LICENSE).

---

<p align="center">
  <b>CANBUS-Stepper</b> • Designed with ❤️ for the Robotics & 3D Printing Community
</p>
