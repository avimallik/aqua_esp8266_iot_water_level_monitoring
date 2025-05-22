# aqua_esp8266_iot_water_level_monitoring

## Overview

This project uses an ESP8266 microcontroller with an ultrasonic sensor and I2C LCD to measure water level in a tank. The system calculates the distance from the sensor to the water surface and provides a water level percentage. It hosts a web server that returns real-time water level data in JSON format over WiFi.

---

## Hardware Components

- ESP8266 Development Board (e.g., NodeMCU)
- Ultrasonic Sensor (HC-SR04 or equivalent)
- 16x2 I2C LCD Display
- Connecting wires and power supply

---

## How It Works

1. The ultrasonic sensor emits a high-frequency sound pulse via the trigger pin.
2. The pulse travels through air, hits the water surface, and reflects back to the sensor.
3. The ESP8266 measures the time (`duration`) for the echo to return.
4. Using this duration, it calculates the distance to the water surface.
5. Converts the distance to a percentage based on the tank height.
6. Displays status and IP on the LCD.
7. Sends JSON-formatted water level data to clients connecting via WiFi.

---

## Mathematical Equations

![Alt text](https://raw.githubusercontent.com/avimallik/aqua_esp8266_iot_water_level_monitoring/refs/heads/master/equation/ultrasonic_distance_measure.PNG)
---

## Software Details

- Arduino core for ESP8266 platform
- Uses ArduinoJson library for JSON formatting
- Uses LiquidCrystal_I2C library for LCD control
- Hosts an HTTP server on port 80 to provide water level data
- WiFi credentials configured in the code

---

## Usage Instructions

1. Connect the ultrasonic sensor and LCD as per pin configuration in the source code.
2. Update WiFi SSID and password in the code.
3. Set the tank height in centimeters in the code.
4. Upload the sketch to the ESP8266 board.
5. Monitor the serial output or LCD for the assigned IP address.
6. Access water level data via HTTP at:

