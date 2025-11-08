# ZeroDerail

**ZeroDerail** is a hobby-built anti-derailment system designed to detect track hazards and communicate early warnings to trains operating in hilly regions. The project uses an autonomous rover that travels ahead of the train, scanning the track for cracks, obstacles, or movement on the line, and transmits alerts over a LoRa link to a receiver module on the train.

---

## Overview

The system is composed of two parts:

- **Rover Unit (ZeroDerail Bot)** – An Arduino Mega–based vehicle that moves along the tracks and performs environmental scanning. It uses ultrasonic sensors to detect cracks and obstacles, PIR sensors to identify motion from animals or humans, and an MPU6050 IMU to detect toppling.  
- **Train Module** – An Arduino Uno–based receiver mounted in the train cab. It receives alerts and periodic updates from the rover using a LoRa RA-02 transceiver and displays or signals them to the operator.

The rover travels roughly 1–1.5 km ahead of the train, matching speed via encoder feedback and LoRa communication. The system is intended for low-speed hilly sections where the likelihood of track obstruction is higher.

---

## Features

- Crack and obstacle detection using ultrasonic sensors  
- Animal and human motion detection using PIR sensors  
- Topple detection via MPU6050 accelerometer/gyroscope  
- LoRa-based long-range two-way communication (bot ↔ train)  
- Speed synchronization between train and rover  
- Audible and visual alerts on detection  
- Modular, low-cost prototype for experimental use

---

## Hardware Summary

**Rover (Arduino Mega 2560):**
- 4 × Ultrasonic sensors (HC-SR04)  
- 2 × PIR sensors  
- MPU6050 accelerometer/gyroscope  
- 2 × DC geared motors with L298N driver  
- LoRa RA-02 transceiver (433 MHz)  
- Wheel encoder  
- Buzzer  
- Powered by 12 V battery (regulator on L298N supplies 5 V to logic)

**Train Module (Arduino Uno):**
- LoRa RA-02 transceiver (433 MHz)  
- 16×2 I²C LCD  
- Wheel encoder  
- Buzzer  
- Powered by 12 V input

Detailed component and wiring information are available in  
[`Hardware/components.md`](Hardware/components.md) and  
[`Hardware/connections.md`](Hardware/connections.md).

---

## Software

The repository includes two sketches:

- [`Code/rover_main/rover_main.ino`](Rover) – Controls sensors, motor logic, LoRa transmission, and hazard detection on the rover.  
- [`Code/train_module_main/train_module_main.ino`](Train Module) – Receives and displays LoRa alerts, measures train speed, and transmits periodic updates to the rover.

Both sketches require:
- **LoRa Library** (Sandeep Mistry)  
- **MPU6050 Library** (e.g., Jeff Rowberg’s I2Cdevlib)  
- Standard Arduino **SPI** and **Wire** libraries  

Upload the respective code to each board using the Arduino IDE.  
Ensure both LoRa modules operate on the same frequency (433 MHz by default).

---

## Notes

- This is a **hobby prototype** built for experimentation and learning.  
  It is **not certified or approved** for any operational railway use.  
- LoRa operates on license-free ISM bands, but users must ensure compliance with regional regulations.  
- Field testing should be performed only in controlled or mock track environments.

---

## Author

**Sannihith Madasu**  

---

## License

Released under the **MIT License**.  
See [`LICENSE`](LICENSE) for details.
