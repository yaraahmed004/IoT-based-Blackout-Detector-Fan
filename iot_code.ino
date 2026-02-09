#include <ESP32Servo.h>          // Servo library for ESP32
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Pins
const int lm35_pin = 32;        // LM35 analog pin
const int servo_pin = 19;       // Servo pin
const int supplyPin = 21;       // Red wire supply simulation

// Servo variables
Servo fanServo;
int angle = 0;
int step = 5;
bool increasing = true;

// Temperature threshold
const float threshold = -1.0;

// BLE UUIDs
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define TEMP_CHAR_UUID      "abcd1234-1a2b-3c4d-5e6f-1234567890ab"
#define FAN_CHAR_UUID       "f1e2d3c4-b5a6-7890-abcd-1234567890ab"

BLECharacteristic *tempCharacteristic;
BLECharacteristic *fanCharacteristic;

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  delay(4000); // Wait 4 seconds for Serial to initialize
  Serial.println("Serial ready, starting BLE...");

  // Initialize servo
  fanServo.attach(servo_pin);
  fanServo.write(angle);

  // Setup red-wire supply pin
  pinMode(supplyPin, INPUT);

  // Initialize BLE
  BLEDevice::init("SmartFan");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  tempCharacteristic = pService->createCharacteristic(
                        TEMP_CHAR_UUID,
                        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
                      );
  tempCharacteristic->addDescriptor(new BLE2902());

  fanCharacteristic = pService->createCharacteristic(
                        FAN_CHAR_UUID,
                        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
                      );
  fanCharacteristic->addDescriptor(new BLE2902());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->start();

  Serial.println("BLE advertising started...");
}

void loop() {
  // Read temperature
  int adc_value = analogRead(lm35_pin);
  float voltage = adc_value * (3.3 / 4095.0); // ESP32 ADC scaling
  float tempC = voltage * 100.0;

  // Read red-wire supply
  int supplyState = digitalRead(supplyPin);

  Serial.print("Temperature = ");
  Serial.print(tempC);
  Serial.print(" °C | Supply wire = ");
  Serial.println(supplyState ? "Plugged" : "Unplugged");

  // Update BLE characteristics
  tempCharacteristic->setValue(tempC);
  tempCharacteristic->notify();
  fanCharacteristic->setValue((!supplyState && tempC >= threshold) ? 1 : 0);
  fanCharacteristic->notify();

  // making fan only work if wire unplugged AND temp above threshold
  if(!supplyState && tempC >= threshold){
    Serial.println("Fan ON (swinging)");

    // swing servo
    if(increasing){
      angle += step;
      if(angle >= 120) increasing = false;
    } else {
      angle -= step;
      if(angle <= 0) increasing = true;
    }

    fanServo.write(angle);
    delay(30);  // fan swing speed
  } else {
    Serial.println("Fan OFF");
    fanServo.write(0);  // servo resting
    delay(700);         // slower loop when off
  }
}
