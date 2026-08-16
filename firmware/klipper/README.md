# 🖨️ Klipper & Katapult (CanBoot) Setup Guide

This guide explains how to compile and flash **Klipper** or **Katapult (CanBoot)** onto the **CANBUS-Stepper v2.1** board to use it as a high-speed CAN bus stepper / toolhead node in 3D printers and CNC machines.

---

## 🛠️ 1. Compiling Katapult Bootloader

Katapult allows future firmware updates to be flashed directly over the CAN bus without plugging in a USB cable.

1. Clone Katapult:
   ```bash
   git clone https://github.com/Arksine/katapult
   cd katapult
   make menuconfig
   ```

2. Configure Katapult settings for **ESP32-S3**:
   - **Micro-controller Architecture**: `Espressif ESP32`
   - **Processor model**: `esp32s3`
   - **Communication interface**: `CAN bus`
   - **CAN RX gpio number**: `47`
   - **CAN TX gpio number**: `48`
   - **CAN bus speed**: `500000` (or `1000000` for 1M)
   - **Status LED pin**: `10`

3. Compile and flash over USB:
   ```bash
   make
   esptool.py --chip esp32s3 -p /dev/ttyACM0 write_flash 0x0 out/katapult.bin
   ```

---

## 🚀 2. Compiling Klipper MCU Firmware

1. Open Klipper menuconfig:
   ```bash
   cd ~/klipper
   make menuconfig
   ```

2. Select the following parameters:
   - **Enable extra low-level configuration options**: `[ * ]`
   - **Micro-controller Architecture**: `Espressif ESP32`
   - **Processor model**: `esp32s3`
   - **Communication interface**: `CAN bus`
   - **CAN RX gpio number**: `47`
   - **CAN TX gpio number**: `48`
   - **CAN bus speed**: `500000` (or your CAN network speed)

3. Compile Klipper:
   ```bash
   make
   ```

4. Flash Klipper over CAN Bus via Katapult:
   ```bash
   python3 ~/katapult/scripts/flashtool.py -i can0 -u <YOUR_CANBUS_UUID> -r
   python3 ~/katapult/scripts/flashtool.py -i can0 -u <YOUR_CANBUS_UUID> -f ~/klipper/out/klipper.bin
   ```

---

## 📄 3. Integrating with `printer.cfg`

Copy the provided [`canbus_stepper_klipper.cfg`](canbus_stepper_klipper.cfg) configuration into your Klipper configuration directory and include it in `printer.cfg`:

```ini
[include canbus_stepper_klipper.cfg]
```
