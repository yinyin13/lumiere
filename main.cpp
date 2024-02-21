#include <Arduino.h>
#include "Adafruit_VEML7700.h"
#include <Wire.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C
Adafruit_VEML7700 veml = Adafruit_VEML7700();

// Initialize variable
int sensorValue;

// Calibration constants (example values)
const float VREF = 3.3;  // Reference voltage in volts
const float SENSITIVITY = 3.5;  // Microphone sensitivity in volts per Pascal (example value)

void setup() {
  Serial.begin(115200);
  Serial.println("VEML7700 demo");

  while (!Serial);    // time to get serial running
  Serial.println(F("BME280 test"));

  unsigned status;

  // Default settings
  status = bme.begin(0x76);
  if (!status) {
    Serial.println(F("BME280 Sensor not found!"));
    while (1) delay(10);
  }

  if (veml.begin()) {
    Serial.println("Found a VEML7700 sensor");
  } else {
    Serial.println("No sensor found ... check your wiring?");
    while (1);
  }

  veml.setGain(VEML7700_GAIN_1);
  veml.setIntegrationTime(VEML7700_IT_100MS);
}

void loop() {
  // Read temperature
  float temperature = bme.readTemperature();
  
  // Read lux
  float lux = veml.readLux();

  // Get the analog value from MAX9814
  sensorValue = analogRead(A0);
  
  // Convert analog value to voltage
  float voltage = sensorValue * (VREF / 1023.0);

  // Convert voltage to sound pressure level (SPL) in Pascals
  float soundPressure = voltage / SENSITIVITY;

  // Convert sound pressure level to decibels (dB)
  float dB = 20 * log10(soundPressure / 0.00002); // 0.00002 Pa is the reference pressure for 0 dB SPL
  
  // Send all data over serial
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Lux: ");
  Serial.print(lux);
  Serial.print(" lux, Sound Pressure Level: ");
  Serial.print(dB);
  Serial.println(" dB");

  delay(1000); // Adjust delay as needed
}
