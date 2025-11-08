// === Arduino Uno (Train) ===
#include <SPI.h>
#include <LoRa.h>

#define NSS 10
#define RST 9
#define DIO0 2

// Encoder setup
const int encoderPin = 3;
const int radius = 5;     // Wheel radius in cm
#define PI 3.14159
const float current_distance = 2 * PI * radius;

volatile int ticks = 0;
volatile unsigned long last_interrupt_time = 0;

unsigned long start_time;
long long int totalDistance = 0;

// LoRa timing
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 5000;  // Send every 5 seconds
const unsigned long listenWindow = 1000;  // Listen for 1 second

void setup() {
  Serial.begin(9600);

  pinMode(encoderPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPin), encoderISR, FALLING);
  start_time = millis();

  LoRa.setPins(NSS, RST, DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (1);
  }

  Serial.println("Train Module Initialized");
}

void loop() {
  float trainSpeed = calculateSpeed();

  if (millis() - lastSendTime >= sendInterval && trainSpeed != -1) {
    lastSendTime = millis();
    sendTrainSpeed(trainSpeed);
    listenForAlerts(listenWindow);
  }
}

float calculateSpeed() {
  noInterrupts();
  int current_ticks = ticks;
  interrupts();

  if (current_ticks >= 20) {
    noInterrupts();
    ticks = 0;
    interrupts();

    unsigned long end_time = millis();
    unsigned long time_elapsed = end_time - start_time;
    start_time = end_time;

    totalDistance += current_distance;
    return (current_distance / time_elapsed) * 1000.0; // cm/s
  }

  return -1;
}

void sendTrainSpeed(float speed) {
  String speedStr = String(speed, 2);
  LoRa.beginPacket();
  LoRa.print(speedStr);
  LoRa.endPacket();
  Serial.print("Sent train speed: ");
  Serial.println(speedStr);
}

void listenForAlerts(unsigned long duration_ms) {
  unsigned long start = millis();
  Serial.println("Listening for alerts...");
  while (millis() - start < duration_ms) {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String incoming = "";
      while (LoRa.available()) {
        incoming += (char)LoRa.read();
      }
      Serial.print("Alert received from bot: ");
      Serial.println(incoming);
    }
  }
  Serial.println("Done listening.");
}

void encoderISR() {
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 5) {
    ticks++;
    last_interrupt_time = interrupt_time;
  }
}