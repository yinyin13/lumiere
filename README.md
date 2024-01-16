# Lumière
A **smart ambient light** for your home 💡 

It will detect the lux level of the environment, and then change the brightness and color of the LED accordingly based on **user profile, mood, or routine**. This calculation will be **handled by ML**. Users can also change the settings manually with a remote control; the adjustments will be **logged and learned**.

## Visualization
![Braindump-41](https://github.com/yinyin13/lumiere/assets/148395165/aa7f0345-1a8c-4b89-a30b-348738f54d1c)

## The Sensor Device (Ceiling Light)
### Components
- VEML7700 sensor
- BME280 sensor
- ESP32 microcontroller
- LED * 2

### Communication
1. Connect the components
2. Program the ESP32 to read temperature and lux data from VEML7700 and BME280
3. Program the ESP32 to send data to Display device via Bluetooth

## The Display Device (Remote Control)
### Components
- Stepper gauge
- ESP32 microcontroller
- LED
- Buttons

### Communication
1. Connect the components
2. ESP32 receives data from the Sensor device
3. Program ESP32 to control the stepper gauge to indicate brightness
4. Program ESP32 to send adjustment commands to the Sensor device

## Diagram
![diagram 001](https://github.com/yinyin13/lumiere/assets/148395165/837b9d80-019b-4136-94b9-cce0888619b1)
