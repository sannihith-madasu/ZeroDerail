# ZeroDerail — Component List

| **Category** | **Component** | **Quantity** | **Description / Purpose** | **Approx. Cost (INR)** |
|---------------|---------------|---------------|-----------------------------|-------------------------|
| **Main Controller** | Arduino Mega 2560 | 1 | Controls the ZeroDerail rover and all sensors | 900 |
| | Arduino Uno | 1 | Controls the train-side module for data reception and display | 450 |
| **Power System** | 12V Battery Pack | 3 | Powers the rover and motors | 900 |
| | L298N Motor Driver | 1 | Dual-channel driver to control two DC motors | 250 |
| | Onboard 5V Regulator (from L298N) | 1 | Provides 5V to Arduino Mega and sensors | — |
| **Sensors** | Ultrasonic Sensor (HC-SR04) | 4 | 2 front for obstacle detection, 2 bottom for crack detection | 300 |
| | PIR Motion Sensor | 2 | Detects moving objects like animals or humans | 200 |
| | MPU6050 (Gyro + Accelerometer) | 1 | Detects tilt or toppling of the bot | 180 |
| **Actuators / Indicators** | DC Geared Motors | 2 | Drive wheels for movement | 500 |
| | Dummy Wheels | 2 | Provide support for rail chassis | 50 |
| | Buzzer | 1 | Audible alert for motion detection | 50 |
| **Communication** | LoRa RA-02 Module | 2 | Long-range transceivers for bot ↔ train communication | 600 |
| **Display (Train Module)** | 16x2 LCD (I2C Interface) | 1 | Displays alerts and train status | 250 |
| **Mechanical / Support** | 4-Wheel Custom Chassis | 1 | Rail-guided structure to hold electronics | — |
| | Standard Wheels (prototype) | 4 | Used in prototype version | — |
| **Miscellaneous** | Jumper Wires, Breadboard, Connectors | — | For circuit assembly | 100 |
| | Switch, Enclosure, Screws | — | For safety and mounting | 100 |

**Total Estimated Cost:** ₹3,300–₹3,800
