# EchoWind: A Connected System for Wind Sensin and Sonified Feedback

This project explores how real-time wind data can be translated into sound through two physical, interactive devices designed for indoor environments — a vibration-driven wind chime and a servo-controlled xylophone. The system aims to enhance environmental perception and foster biophilic interaction in architectural spaces.

## 📡 System Overview

- **Sensor Node**: An anemometer reads wind speed and publishes real-time values to an MQTT broker.
- **Actuator Nodes**: Two microcontrollers subscribe to the wind topic and sonify the data:
  - **Wind Chime**: Uses a vibration motor to activate metal tubes based on wind intensity.
  - **Xylophone**: Uses 8 MG90S servos to strike keys mapped to wind levels.

## ⚙️ Hardware

- **Anemometer**: A weatherproof anemometer with a range of 0-100 kph.
- **Wind Chime**: A vibration motor driven by a MKR1010 board.
- **Xylophone**: An ESP32-C6 board with 8 MG90S servos.
- **Power Supply**: Three 5V 2A power supply, work with lithunimous batteries as well.

## 🔌 Connections
