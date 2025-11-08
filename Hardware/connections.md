# ZeroDerail — Connections and Pin Mapping

## 🧠 ZeroDerail Rover (Arduino Mega)

| **Component** | **Pin(s)** | **Type** | **Description** |
|----------------|-------------|-----------|-----------------|
| **Front Ultrasonic Left (HC-SR04)** | Trig → 22, Echo → 23 | Digital | Obstacle detection (left front) |
| **Front Ultrasonic Right (HC-SR04)** | Trig → 24, Echo → 25 | Digital | Obstacle detection (right front) |
| **Bottom Ultrasonic Left (HC-SR04)** | Trig → 26, Echo → 27 | Digital | Crack detection (left) |
| **Bottom Ultrasonic Right (HC-SR04)** | Trig → 28, Echo → 29 | Digital | Crack detection (right) |
| **PIR Sensor 1** | 30 | Digital (Input) | Detects motion (left side) |
| **PIR Sensor 2** | 31 | Digital (Input) | Detects motion (right side) |
| **Buzzer** | 32 | Digital (Output) | Audible alert |
| **Motor Driver L298N** | IN1 → 33, IN2 → 34, IN3 → 35, IN4 → 36, ENA → A0, ENB → A1 | PWM / Digital | Motor control for left/right drive motors |
| **Motor Power (L298N)** | +12V / GND | Power | Drives motors and onboard regulator |
| **Encoder Sensor** | 2 (Interrupt) | Digital | Measures wheel rotation for distance & speed |
| **MPU6050 (I2C)** | SDA → 20, SCL → 21 | I2C | Detects tilt and toppling |
| **LoRa RA-02** | NSS → 10, RST → 9, DIO0 → 2, MISO → 50, MOSI → 51, SCK → 52 | SPI | Long-range communication with train module |
| **Power Input** | +12V Battery Pack | Power | Main supply for rover |
| **5V Output (from L298N Regulator)** | To Mega VIN or 5V | Power | Powers Arduino Mega |

---

## 🚆 Train Module (Arduino Uno)

| **Component** | **Pin(s)** | **Type** | **Description** |
|----------------|-------------|-----------|-----------------|
| **LoRa RA-02** | NSS → 10, RST → 9, DIO0 → 2, MISO → 12, MOSI → 11, SCK → 13 | SPI | Communicates with bot over long range |
| **16x2 LCD (I2C)** | SDA → A4, SCL → A5 | I2C | Displays status and alerts |
| **Buzzer** | 4 | Digital (Output) | Sounds on incoming alerts |
| **Encoder Sensor** | 3 (Interrupt) | Digital | Measures train wheel rotation for speed sync |
| **Power Input** | +12V → VIN / barrel jack | Power | Powered from train’s supply |
| **Ground** | Common GND | Power | Shared between all components |

---

## 🔌 Power Distribution Summary

| **Module** | **Power Source** | **Voltage** | **Notes** |
|-------------|------------------|--------------|------------|
| Arduino Mega (Bot) | From L298N Regulator | 5V | L298N 5V output powers Mega |
| L298N Driver | 12V Battery | 12V | Drives DC motors |
| LoRa RA-02 (Bot) | From Mega 3.3V | 3.3V | Ensure voltage divider on NSS, SCK, MOSI lines |
| Sensors (Ultrasonic, PIR) | From Mega 5V | 5V | Shared via breadboard rail |
| MPU6050 | From Mega 5V | 5V | I2C interface |
| Arduino Uno (Train) | Train power 12V | 12V | Via barrel jack or VIN |
| LoRa RA-02 (Train) | From Uno 3.3V | 3.3V | With logic-level adjustment |
| LCD (I2C) | From Uno 5V | 5V | For display |

---

## ⚙️ Notes

- **Common Ground**: Always connect the ground of all modules (Mega, L298N, LoRa, sensors) together.  
- **Voltage Safety**: LoRa RA-02 is **3.3V logic**, so use a voltage divider or logic-level shifter if necessary.  
- **Motor Noise**: Add a 470µF capacitor near the L298N power input to smooth voltage dips.  
- **Testing Tip**: Test each sensor and LoRa link separately before integrating full system.  

