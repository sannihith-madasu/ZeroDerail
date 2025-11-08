#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>
#include <MPU6050.h>

#define PI 3.14159

// Ultrasonic Sensor Pins
const int trig_front_left = 22;
const int echo_front_left = 23;
const int trig_front_right = 24;
const int echo_front_right = 25;
const int trig_bottom_left = 26;
const int echo_bottom_left = 27;
const int trig_bottom_right = 28;
const int echo_bottom_right = 29;

// PIR Sensors and Buzzer
const int PIR1 = 30;
const int PIR2 = 31;
const int buzzer = 32;

// Motor Driver Pins
const int IN1 = 33;
const int IN2 = 34;
const int IN3 = 35;
const int IN4 = 36;
const int ENA = A0;
const int ENB = A1;

// Encoder Pin
const int encoderPin = 2;
const int radius = 5; // cm
const float distancePerRotation = 2 * PI * radius;

// Thresholds and Variables
const int crack_threshold = 18; // cm
int motorSpeed = 127; // PWM (0-255)

volatile int ticks = 0;
volatile unsigned long last_interrupt_time = 0;
unsigned long start_time = 0;
long long int totalDistance = 0;
float latestBotSpeed = 0.0;
float latestTrainSpeed = 0.0;
unsigned long lastSpeedUpdate = 0;

MPU6050 mpu;

void setup() {
  Serial.begin(9600);

  pinMode(trig_front_left, OUTPUT); pinMode(echo_front_left, INPUT);
  pinMode(trig_front_right, OUTPUT); pinMode(echo_front_right, INPUT);
  pinMode(trig_bottom_left, OUTPUT); pinMode(echo_bottom_left, INPUT);
  pinMode(trig_bottom_right, OUTPUT); pinMode(echo_bottom_right, INPUT);

  pinMode(PIR1, INPUT); pinMode(PIR2, INPUT);
  pinMode(buzzer, OUTPUT); noTone(buzzer);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  analogWrite(ENA, 0); analogWrite(ENB, 0);

  pinMode(encoderPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPin), encoderISR, FALLING);
  start_time = millis();

  LoRa.setPins(10, 9, 2);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa failed.");
    while (1);
  }

  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 fail.");
    while (1);
  }
}

long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  return duration * 0.034 / 2;
}

float calculateSpeed() {
  noInterrupts();
  int current_ticks = ticks;
  interrupts();

  if (current_ticks >= 20) {
    noInterrupts(); ticks = 0; interrupts();
    unsigned long now = millis();
    unsigned long delta = now - start_time;
    start_time = now;

    latestBotSpeed = (distancePerRotation / delta) * 1000;
    totalDistance += distancePerRotation;
    lastSpeedUpdate = now;
    return latestBotSpeed;
  }
  return latestBotSpeed;
}

void encoderISR() {
  unsigned long now = millis();
  if (now - last_interrupt_time > 5) {
    ticks++;
    last_interrupt_time = now;
  }
}

void receiveTrainSpeed() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incoming = "";
    while (LoRa.available()) incoming += (char)LoRa.read();
    latestTrainSpeed = incoming.toFloat();
    Serial.print("Train Speed: ");
    Serial.println(latestTrainSpeed);
  }
}

void matchSpeed() {
  float botSpeed = calculateSpeed();
  float diff = botSpeed - latestTrainSpeed;
  if (abs(diff) > 2.0) {
    motorSpeed += (diff < 0) ? 5 : -5;
    motorSpeed = constrain(motorSpeed, 0, 255);
  }
}

void wheelForward() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void loraTransmit(String message) {
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
  delay(100);
}

bool isTopple() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  float gX = ax / 16384.0;
  float gY = ay / 16384.0;
  float gZ = az / 16384.0;
  float pitch = atan2(gY, sqrt(gX * gX + gZ * gZ)) * 180.0 / PI;
  float roll = atan2(-gX, gZ) * 180.0 / PI;
  if (abs(pitch) > 45 || abs(roll) > 45) {
    loraTransmit("Bot Toppled");
    return true;
  }
  return false;
}

void loop() {
  receiveTrainSpeed();
  matchSpeed();
  wheelForward();

  // Obstacle detection (presence-based)
  long durationLeft = pulseIn(echo_front_left, HIGH, 30000);
  long durationRight = pulseIn(echo_front_right, HIGH, 30000);
  if (durationLeft > 0 || durationRight > 0) {
    stopMotors();
    loraTransmit("Obstacle detected");
  }

  // PIR - detect moving bodies like animals and humans and trigger a buzzer
  if (digitalRead(PIR1) || digitalRead(PIR2)) {
    tone(buzzer, 1000);
    delay(1000);
    noTone(buzzer);
  }

  // Crack detection
  static bool crackOngoing = false;
  static unsigned long crackStart = 0;
  long bottomLeft = measureDistance(trig_bottom_left, echo_bottom_left);
  long bottomRight = measureDistance(trig_bottom_right, echo_bottom_right);
  bool crackDetected = (bottomLeft > crack_threshold || bottomRight > crack_threshold);

  if (crackDetected && !crackOngoing) {
    crackStart = millis();
    crackOngoing = true;
  }
  if (!crackDetected && crackOngoing) {
    crackOngoing = false;
    unsigned long crackEnd = millis();
    float crackDuration = crackEnd - crackStart;
    float crackLength = (crackDuration / 1000.0) * latestBotSpeed * 10;
    if (crackLength > 20.0) {
      Serial.print("Crack: ");
      Serial.println(crackLength);
      loraTransmit("Crack detected");
      stopMotors();
    }
  }

  if (isTopple()) {
    loraTransmit("Bot toppled! Potential danger!");
    stopMotors();
    delay(100);
  }
}