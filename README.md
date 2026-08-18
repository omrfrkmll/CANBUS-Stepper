# ⚡ CANBUS-Stepper

<p align="center">
  <b>ESP32-S3 Based Closed-Loop Smart CAN Bus Stepper Motor Driver Board (v1.0)</b>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Firmware-MIT%20License-green?style=for-the-badge" alt="Firmware License"/>
  <img src="https://img.shields.io/badge/Hardware-Reference%20(CC%20BY--NC--SA%204.0)-blue?style=for-the-badge&logo=open-source-initiative&logoColor=white" alt="Hardware License"/>
  <img src="https://img.shields.io/badge/Schematic-PDF%20v1.0-red?style=for-the-badge&logo=adobeacrobatreader&logoColor=white" alt="Schematic PDF"/>
  <img src="https://img.shields.io/badge/MCU-ESP32--S3-orange?style=for-the-badge&logo=espressif&logoColor=white" alt="MCU ESP32-S3"/>
  <img src="https://img.shields.io/badge/Driver-TMC2209--LA-007ACC?style=for-the-badge" alt="Driver TMC2209"/>
  <img src="https://img.shields.io/badge/Bus-CAN%20Bus%20%2F%20CAN%20FD-orange?style=for-the-badge" alt="Bus CAN"/>
  <img src="https://img.shields.io/badge/Encoder-AS5600%20(12--Bit)-4BC51D?style=for-the-badge" alt="Encoder AS5600"/>
</p>

<p align="center">
  <a href="#-overview">Overview</a> •
  <a href="#-development-status">Development Status</a> •
  <a href="#-hardware-features--specifications">Hardware Specs</a> •
  <a href="#-pcb--hardware-preview">PCB Preview</a> •
  <a href="#-schematics--mechanical-cad">Schematic & CAD</a> •
  <a href="#-firmware--control-sdk">Firmware & SDK</a> •
  <a href="#-official-hardware--custom-engineering">Official Hardware & Services</a> •
  <a href="#-license">License</a>
</p>

---

## 📌 Overview

**CANBUS-Stepper** is a high-performance smart stepper motor driver board powered by the **ESP32-S3** dual-core microcontroller. Designed specifically for 3D printer toolheads, robotic actuators, AGVs, CNC axes, and industrial automation, this board integrates:

- **Ultra-Quiet Motor Drive**: Trinamic TMC2209-LA driver with StealthChop2™, SpreadCycle™, and StallGuard4™ sensorless homing.
- **Closed-Loop Position Feedback**: High-precision AS5600 12-bit magnetic rotary encoder mounted coaxially on the bottom layer to eliminate missed steps.
- **Native CAN Bus Communication**: Automotive-grade TI TCAN1051V transceiver supporting high-speed CAN and CAN FD logic.
- **Industrial Wide-Input Power**: High-efficiency TI LMR33630 36V / 3A synchronous step-down DC-DC buck regulator + ultra-low-noise 3.3V LDO.
- **Multi-Stage Protection Circuits**: Reverse polarity P-MOSFET, transient voltage suppressors (TVS), ESD diode arrays, and resettable PPTC fuses.
- **Compact NEMA 17 Footprint**: Fits directly onto the rear face of standard 42mm x 42mm NEMA 17 stepper motors (40.3mm x 40.3mm profile with corner chamfers and M3 mounting holes).

---

## 🚧 Development Status

| Component | Status | Details |
| :--- | :---: | :--- |
| **Schematic Design** | ✅ Complete | Hierarchical schematic design, [PDF export available](docs/schematics/CANBUS_Stepper_v1.0_Schematic.pdf) |
| **PCB Layout (v1.0)** | ✅ Complete | 2-Layer 1.6mm FR4, DRC clean, impedance-tuned CAN bus differential pairs |
| **Component Mapping (BOM)** | ✅ Complete | 43 unique lines / 67 components 100% mapped with LCSC part numbers |
| **3D CAD STEP Models** | ✅ Complete | Exported 3D assembly ([`step/canbus-stepper.step`](step/canbus-stepper.step)) ready for enclosure design |
| **Physical Prototyping** | 🔄 In Progress | Prototype PCBA fabrication & physical hardware bring-up |
| **Firmware & Control SDK** | 🔄 In Progress | ESP32-S3 TWAI CAN driver, closed-loop PID loop & Klipper node support |

---

## 🛠️ Hardware Features & Specifications

### 🧠 Core System & Processing
- **MCU**: **ESP32-S3-MINI-1-N4R2** (Dual-core Xtensa® 32-bit LX7 @ 240 MHz, 2.4 GHz Wi-Fi 4 + Bluetooth 5 LE, 4 MB Flash, 2 MB Quad SPI PSRAM).
- **USB Interface**: Native Full-Speed USB 2.0 via Type-C connector with hardware CDC serial console & JTAG debugging support.

### ⚡ Motor Drive & Closed-Loop Regulation
- **Stepper Driver**: **Trinamic TMC2209-LA** (VQFN-28 package with thermal vias).
  - Operating Voltage: Up to 28V DC (recommended 12V – 24V).
  - Current Capability: 2.0A RMS (2.8A Peak) motor phase current.
  - Features: StealthChop2™ (silent operation), SpreadCycle™ (high dynamic torque), StallGuard4™ (load measurement & sensorless homing), CoolStep™ (energy saving).
  - Interface: UART control for dynamic microstepping configuration (up to 256 microsteps) and real-time current scaling.
- **Position Sensor**: **AMS AS5600-ASOT** 12-Bit On-Axis Magnetic Rotary Encoder.
  - Position Resolution: 4096 steps/rev (0.087° resolution).
  - Mounting: Centered directly on PCB bottom layer for coaxial alignment with motor shaft diametral magnet.
  - Interface: Fast-mode I2C (400 kHz).

### 🌐 Bus Communication & Power Regulation
- **CAN Bus Transceiver**: **TI TCAN1051VDRBRQ1** 5V CAN FD Transceiver with 3.3V V_IO logic interface and ±58V bus fault protection.
- **Main DC-DC Buck Converter**: **TI LMR33630CRNXR** Synchronous Step-Down Regulator (3.8V – 36V input range, 3.0A continuous output @ 2.1 MHz switching frequency).
- **MCU & Analog LDO**: **TI TLV75533PDRVR** 500mA ultra-low-dropout 3.3V regulator providing clean power to ESP32-S3 and sensors.
- **Bulk Filtering**: 100µF / 35V Low-ESR aluminum electrolytic bulk capacitor (6.3mm x 5.8mm SMD) for motor switching current ripple suppression.

### 🛡️ Protection & Safety Architecture
- **Reverse Polarity Protection**: **Alpha & Omega AON7407** (-30V / -30A, 8.5mΩ R_DS(on) P-Channel MOSFET) with Zener gate clamping.
- **Overcurrent Protection**:
  - 1.5A / 24V Resettable PPTC fuse (`1210L150/24WR`) on main 24V DC input rail.
  - 500mA / 16V Resettable PPTC fuse (`SMDH0805B050TF`) on USB VBUS power line.
- **Transient & ESD Protection**:
  - **PESD1CAN** TVS diode array on CAN Bus differential lines (CAN_H / CAN_L).
  - **SMAJ28A** 28V / 400W Transient Voltage Suppressor on main 24V DC input.
  - **STMicroelectronics USBLC6-2P6** ultra-low capacitance ESD protection array on USB D+/D- lines.
  - **MBR0520** Schottky diode for safe USB/Main power OR-ing.

### 🔌 Connectors, Ports & Controls
| Connector / Component | Type / Footprint | Function |
| :--- | :--- | :--- |
| **USB-C** | 16-Pin USB Type-C Receptacle | Firmware flashing, USB CDC serial debugging & power |
| **24V Power In / Out** | 2x Molex Micro-Fit 3.0 (436500209) 2-Pin | 24V Main power input & daisy-chain pass-through |
| **Motor Output** | JST PH 4-Pin SMD (S4B-PH-SM4-K-TB) | 4-wire bipolar stepper motor phases (A1, A2, B1, B2) |
| **Qwiic / I2C Port** | JST SH 4-Pin (1.0mm) Horizontal | External I2C sensors, OLED displays & expansions (3.3V, GND, SDA, SCL) |
| **AUX Port** | JST SH 4-Pin (1.0mm) Horizontal | Auxiliary GPIOs for endstops, bed probes, or toolhead sensors |
| **CAN Bus In / Out** | 2x JST SH 2-Pin (SM02B-SRSS-TB) | CAN Bus differential input & daisy-chain pass-through (CAN_H, CAN_L) |
| **OLED Display FPC** | Amphenol SFV30R-2STBE1HLF (30-Pin 0.5mm) | Bare COG OLED display panel interface (SSD1306) |
| **CAN Termination** | SMD Slide Switch (`MSK12C01G15-W`) | Switchable 120Ω CAN bus termination resistor |
| **Buttons & Indicators** | 2x Tactile Push-Buttons + 3x LEDs | RESET, BOOT/User buttons; Power, Driver, and CAN Status LEDs |

---

## 🖼️ PCB & Hardware Preview

<p align="center">
  <img src="docs/assets/pcb_top.png?v=2" alt="CANBUS-Stepper PCB Top Side" width="48%"/>
  &nbsp;&nbsp;
  <img src="docs/assets/pcb_bottom.png?v=2" alt="CANBUS-Stepper PCB Bottom Side" width="48%"/>
  <br>
  <i>Figure 1: CANBUS-Stepper v1.0 Component Layout — Top Layer (Left) & Bottom Layer (Right)</i>
</p>

---

## 📄 Schematics & Mechanical CAD

To enable seamless third-party software integration, driver development, and custom enclosure modeling, official hardware reference documents are provided:

1. **Schematic Circuit Diagrams (PDF)**:
   - Complete multi-sheet reference schematics are available in [`docs/schematics/CANBUS_Stepper_v1.0_Schematic.pdf`](docs/schematics/CANBUS_Stepper_v1.0_Schematic.pdf).
   - Details MCU pinouts, TMC2209 UART buses, CAN transceiver wiring, protection circuits, and sensor connectors.
2. **3D Mechanical STEP Assembly**:
   - Ready-to-import 3D CAD model: [`step/canbus-stepper.step`](step/canbus-stepper.step) (28.9 MB).
   - Accurate component heights, connector positions, and NEMA 17 rear mounting hole spacing (31.0mm x 31.0mm diagonal M3 holes).

---

## 📁 Repository Structure

```
CANBUS-Stepper/
├── README.md                      # Main project documentation & specifications
├── LICENSE                        # CC BY-NC-SA 4.0 (Hardware Reference) & MIT (Software)
├── .gitignore                     # Git ignore rules
├── docs/
│   ├── assets/                    # 3D PCB renders, board photos, and diagrams
│   └── schematics/                # Full PDF Schematic & Pinout reference documentation
│       ├── README.md              # Sheet breakdown and integration guide
│       └── CANBUS_Stepper_v1.0_Schematic.pdf
├── firmware/                      # ESP32-S3 Firmware, Protocol & SDK
│   ├── include/                   # Drivers, control loops, and headers
│   ├── src/                       # Motion controller, TWAI CAN, AS5600, TMC2209
│   ├── klipper/                   # Klipper toolhead configuration & docs
│   ├── platformio.ini             # PlatformIO build configuration
│   └── README.md                  # Firmware architecture & roadmap
└── step/                          # 3D CAD STEP Files for mechanical integration
    ├── canbus-stepper.step        # Full 3D PCB assembly STEP model (28.9 MB)
    └── README.md                  # Mounting specifications & CAD guide
```

---

## 🧠 Firmware & Control SDK

The ESP32-S3 microcontroller firmware is developed under `firmware/` and targets both standalone robotic motion control and 3D printing ecosystems:

- **TWAI CAN Protocol**: High-speed command parser for position target streaming, velocity profiling, and status telemetry.
- **Closed-Loop PID Loop**: High-frequency reading from the AS5600 magnetic encoder with real-time step error correction and missed step prevention.
- **Klipper CAN Toolhead Support**: Compatibility with Klipper firmware for plug-and-play toolhead CAN node integration.
- **ROS2 & Micro-ROS**: Direct actuator node integration for robotic arms, wheeled rovers, and multi-axis mechanisms.
- **Open SDK**: Python and C++ driver libraries for controlling motors over USB-to-CAN adapters.

---

## 🛒 Official Hardware, Kits & Custom Engineering

Ensure guaranteed hardware quality and support ongoing development through **Official Pre-Assembled Hardware & Engineering Services**:

### 📦 Official Plug-and-Play Kits
- 🛠️ **Factory-Tested Boards**: 100% SMT assembled, flashed with factory test firmware, and validated under motor load.
- 🧲 **Complete Motor Kits**: Bundled with high-temperature NEMA 17 stepper motors, matched diametrically magnetized shaft magnets, 3D printed mounting brackets, and pre-crimped Molex/JST wire harnesses.
- ☕ **Support Ongoing Development**: Official hardware sales directly fund continuous firmware development, Klipper modules, and future hardware revisions.

### 🏢 B2B Custom Robotics & Turnkey Engineering
Need customized hardware or industrial integration for your commercial robotics or automation project?
- **Custom PCB Redesigns** (Multi-axis drivers, 48V high-voltage input, RS485 / EtherCAT / DroneCAN).
- **Industrial Firmware & ROS2 / Micro-ROS Drivers**.
- **Volume Batch Manufacturing & PCBA Sourcing**.

👉 *For pre-orders, volume batch requests, or custom engineering consulting, please reach out via GitHub Issues or contact the maintainer.*

---

## 📜 License

This project follows a **Hybrid Open-Software & Reference-Hardware** model:

- 🛡️ **Hardware Reference (Schematics, Pinouts, Mechanical Specs & 3D STEP)**: [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/).
- 💻 **Firmware & Software Stack**: [MIT License](LICENSE).
- 🏷️ **Trademarks & Official Branding**: The "CANBUS-Stepper" and "ILTERAY" names and logos represent official tested hardware.

---

<p align="center">
  <b>CANBUS-Stepper</b> • Open-Source Smart Motion Control
</p>

