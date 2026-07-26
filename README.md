# ⚡ CANBUS-Stepper

<p align="center">
  <b>ESP32-S2 Based Closed-Loop CAN Bus Stepper Motor Driver Board</b>
</p>

<p align="center">
  <a href="#-overview">Overview</a> •
  <a href="#-hardware-features">Hardware Features</a> •
  <a href="#-repository-structure">Repository Structure</a> •
  <a href="#-getting-started">Getting Started</a> •
  <a href="#-license">License</a>
</p>

---

## 📌 Overview

**CANBUS-Stepper** is an open-source, high-performance smart stepper motor driver board built around the **ESP32-S2** microcontroller. Designed specifically for 3D printer toolheads, robotic actuators, and CNC automation, this board integrates a native CAN Bus transceiver (TCAN1044V), magnetic position encoder feedback (AS5600), power regulation (MP2315 buck converter), and stepper motor driving into a compact NEMA 17 footprint.

---

## 🛠️ Hardware Features

- **Microcontroller**: ESP32-S2 (Single-core Xtensa LX7 @ 240MHz, Native USB 2.0, TWAI / CAN Bus support).
- **CAN Bus Transceiver**: TCAN1044VDDFRQ1 5V CAN Transceiver with high ESD and bus fault protection.
- **Power Management**: MP2315GJ High-efficiency step-down buck converter (Wide DC input voltage range).
- **Position Feedback**: AS5600 Magnetic Rotary Encoder (I2C interface) for accurate absolute angle sensing and closed-loop control.
- **Connectivity & I/O**:
  - USB Type-C receptacle for programming, flashing, and serial debugging.
  - Molex Micro-Fit 3.0 & JST connectors for CAN Bus power/data and motor phase outputs.
  - SPDT Slide switches for boot mode and power control.

---

## 📁 Repository Structure

```
CANBUS-Stepper/
├── README.md                      # Main project documentation
├── .gitignore                     # Git ignore rules for KiCad and build outputs
├── docs/
│   └── assets/                    # Board renders, diagrams, and documentation assets
├── pcb/                           # KiCad PCB Hardware Design Files
│   ├── canbus-stepper.kicad_pro   # KiCad Project file
│   ├── canbus-stepper.kicad_sch   # Schematic diagram
│   ├── canbus-stepper.kicad_pcb   # PCB Layout
│   ├── sym-lib-table              # Symbol library table
│   ├── fp-lib-table               # Footprint library table
│   ├── *.kicad_sym                # Custom symbol libraries (AS5600, TCAN1044, MP2315, etc.)
│   ├── CANBUS-Stepper.pretty/     # Custom footprint library
│   ├── 3dmodels/                  # Component 3D STEP models for KiCad rendering
│   └── bom/                       # Bill of Materials & Interactive HTML iBOM (ibom.html)
├── firmware/                      # ESP32-S2 Stepper Driver Firmware
│   └── README.md                  # Firmware architecture & CAN bus protocol roadmap
└── step/                          # Board 3D CAD STEP Files
    └── README.md                  # Exported overall PCB 3D STEP model for mechanical integration
```

---

## 🔧 Getting Started

### Opening the PCB Project in KiCad

1. Download and install [KiCad 7.0 or later](https://www.kicad.org/).
2. Clone this repository:
   ```bash
   git clone https://github.com/your-username/CANBUS-Stepper.git
   cd CANBUS-Stepper
   ```
3. Open `pcb/canbus-stepper.kicad_pro` inside KiCad.
4. To view the Interactive Bill of Materials, open `pcb/bom/ibom.html` in any web browser.

---

## 📜 Firmware Roadmap

Firmware development for the ESP32-S2 is structured under `firmware/`. Key roadmap milestones include:
- **TWAI CAN Bus Protocol**: Standardized command set for motor movement, status telemetry, and configuration.
- **Closed-Loop Control**: AS5600 encoder feedback loop for stall detection and precise step regulation.

---

## 🤝 Contributing & License

Contributions, bug reports, and feature suggestions are highly welcome! Feel free to open an Issue or submit a Pull Request.

Distributed under the MIT License.
