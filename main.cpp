#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_VEML7700.h>
#include <Adafruit_NeoPixel.h>

#define BME_SDA 21
#define BME_SCL 22
#define SEALEVELPRESSURE_HPA (1013.25)
#define NUMPIXELS 60 
#define PIXELPIN 6  

Adafruit_BME280 bme; // I2C
Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800); // NeoPixel 

WiFiMulti WiFiMulti;
WiFiClientSecure client;

#define microphonePin 34

void setup() {
  Serial.begin(115200);
  WiFiMulti.addAP("Your_SSID", "Your_Password");  // Set your SSID and Password

  Wire.begin();

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    delay(1000);
  }

  if (!veml.begin()) {
    Serial.println("Failed to initialize VEML7700, check wiring!");
    while(1);
  }

  pinMode(microphonePin, INPUT);
  pixels.begin(); // Initialize the NeoPixel library.

  while(WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Connected");
}

void loop() {
  int micValue = analogRead(microphonePin);
  float temperature = bme.readTemperature();  // Temperature
  int light = veml.readLux();  // Ambient light level

  uint8_t adjustedBrightness = map(temperature, -10, 40, 0, 255); // Assuming the temperature will range from -10 to 40
  if (light < 300) adjustedBrightness = 255; // If ambient lux level is low, make it the brightest

  pixels.setBrightness(adjustedBrightness);

  // Set color and light up the strip
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, adjustedBrightness, 0)); // Green for this example
  }
  pixels.show();

  // Now it sends micValue, temperature and light data.
  String post_data = "audio_data=" + String(micValue) + "&temperature=" + String(temperature) + "&light_level=" + String(light);

  if(client.connect("Your_Cloud_Server_URL", 443)){  
    client.println("POST /api/v1/audio HTTP/1.1");
    client.println("Host: <your-cloud-server>");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(post_data.length());
    client.println();
    client.print(post_data);
  }

  client.flush();
  client.stop();
  delay(15000);
}