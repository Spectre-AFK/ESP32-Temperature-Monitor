# ESP32 Temperature Monitor

This project demonstrates an IoT temperature monitoring system using an ESP32 microcontroller. The ESP32 reads temperature data from a DS18B20 sensor, displays it on an OLED screen, and publishes the readings to an MQTT broker. While only the ESP32 code is provided here, this setup is designed to integrate with a broader IoT ecosystem.

Developed By: Aaron Jauregui, 03-10-2026

## Architecture Overview

The ESP32 acts as an edge device in an IoT pipeline:

1. **ESP32 Device**: Collects temperature data and publishes it via MQTT.
2. **MQTT Broker**: Typically running on a Raspberry Pi (RPi), receives messages from the ESP32.
3. **Raspberry Pi**: Hosts the MQTT broker and can run additional services like Node-RED for data processing and routing.
4. **Cloud Integration**: Node-RED on the RPi can forward data to Cloudflare (e.g., for secure tunneling or edge computing) and other cloud services as needed.

```
[ESP32] --MQTT--> [RPi with MQTT Broker] --Node-RED--> [Cloudflare / Other Services]
```

This architecture allows for scalable, decoupled communication where the ESP32 focuses on sensing and publishing, while the RPi handles data aggregation, processing, and cloud connectivity.

## Features

- **WiFi Connectivity**: Uses WiFiManager for easy WiFi setup and connection.
- **Temperature Sensing**: Reads temperature from a DS18B20 sensor connected via OneWire.
- **OLED Display**: Shows connection status and current temperature readings.
- **MQTT Publishing**: Sends temperature data to an MQTT broker every 5 seconds.
- **Error Handling**: Displays sensor errors and connection statuses on the OLED.

## Hardware Requirements

- ESP32 development board
- DS18B20 temperature sensor
- SSD1306 OLED display (128x64 pixels)
- Connecting wires (for sensor and display)

## Software Requirements

- Arduino IDE with ESP32 board support
- Required libraries:
  - WiFi
  - WiFiManager
  - Wire
  - Adafruit_GFX
  - Adafruit_SSD1306
  - OneWire
  - DallasTemperature
  - PubSubClient

## Setup Instructions

1. **Install Libraries**: In Arduino IDE, install the required libraries via Library Manager.

2. **Configure MQTT Broker**: Ensure your MQTT broker (e.g., Mosquitto on RPi) is running at `192.168.50.70` on port 1883. Update `mqtt_server` and `mqtt_topic` in the code if needed.

3. **Upload Code**: Connect your ESP32 to your computer, select the appropriate board in Arduino IDE, and upload the `main.ino` sketch.

4. **Initial WiFi Setup**: On first boot, the ESP32 will create an access point named "TempMonitor_Setup". Connect to it and configure your WiFi network.

5. **Monitor Output**: Use the Serial Monitor to view debug information.

## Usage

Once set up, the ESP32 will:
- Connect to WiFi automatically.
- Display connection status on the OLED.
- Read temperature every 5 seconds and display it.
- Publish temperature data to the MQTT topic "spectre/temp".

On the RPi side (not included here):
- Run an MQTT broker to receive messages.
- Use Node-RED to subscribe to the topic, process data, and forward to Cloudflare or other endpoints.

## MQTT Details

- **Broker**: 192.168.50.70:1883
- **Topic**: spectre/temp
- **Message Format**: Temperature value as a string (e.g., "72.50")

## Troubleshooting

- If WiFi connection fails, the device enters AP mode for reconfiguration.
- Sensor errors are displayed as "SENSOR ERR" on the OLED.
- Check Serial output for detailed error messages.

## Future Enhancements

- Add more sensors (humidity, pressure).
- Implement OTA updates for the ESP32.
- Enhance Node-RED flows for advanced data processing and cloud integration.

For questions or contributions, feel free to reach out!