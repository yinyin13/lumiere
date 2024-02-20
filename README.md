# Lumière
A **smart ambient light** for your home 💡 

The Sensor Device utilizes sensors such as VEML7700 and BME280 to **monitor ambient conditions like light levels and temperature**, while also incorporating a MAX9814 microphone for **audio input**. The audio input will be sent to Cloud for processing using **speech recognition**. 

The Display Device provides user interaction, featuring an **encoder for brightness control** and a **stepper gauge to indicate current light modes** like work or relax. Together, Lumiere intelligently adjusts lighting based on environmental factors, user preferences, and emotional states, offering a dynamic and responsive lighting experience.

## Visualization
![Braindump-41](https://github.com/yinyin13/lumiere/assets/148395165/67c6a3e0-1792-467d-a069-34b5398c3784)

## The Sensor Device (Ceiling Light)
### Components
- VEML7700 sensor
- BME280 sensor
- MAX9814 mic
- ESP32S3 microcontroller
- NeoPixel RGB LED strips
- 5mm LED

### Communication
1. Connect the components
2. Program the ESP32 to read temperature and lux data from VEML7700 and BME280
3. Program the ESP32 to read audio input from MAX9814
4. Program the ESP32 to send data to Display device via WiFi

## The Display Device (Remote Control)
### Components
- SPDT Switch (On/Off)
- X27 168 stepper gauge
- LED 0805
- Encoder
- ESP32S3 microcontroller
- Battery

### Communication
1. Connect the components
2. Program the ESP32 to control the stepper gauge to indicate current mode
3. Program the ESP32 to send adjustment commands input from the encoder to the Sensor device

## Diagrams
### System Architecture
![3 proj ideas 001](https://github.com/yinyin13/lumiere/assets/148395165/63fca96c-4c9e-4e6e-80c7-d26062a6f276)

### Flow Chart
![3 proj ideas 002](https://github.com/yinyin13/lumiere/assets/148395165/06ed2034-7750-4438-ad22-6f5ba9df3dd4)
