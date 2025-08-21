# 🌬️ EchoWind: A Connected System for Wind Sensing and Sonified Feedback

This project explores how **real-time wind data** can be translated into **sound** through two interactive physical devices — a **vibration-driven wind chime** and a **servo-controlled xylophone**.  
The system aims to **enhance environmental perception** and foster **biophilic interaction** in architectural spaces.

<div align="center">
  <img src="https://raw.githubusercontent.com/JY-SHENNNN/Dissertation/refs/heads/main/Img/device.jpg" width="800" height="400">
</div>

---

## 🎯 Background & Motivation
Humans in indoor environments have limited perception of natural wind.  
EchoWind bridges this sensory gap by **sonifying wind speed** detected near the building, creating a tangible connection between **outdoor natural phenomena** and **indoor experiences**.  
The project draws inspiration from *biophilic design* principles and aims to provide:  
- **Sensory enrichment** in architectural spaces  
- **Ambient awareness** of changing weather  
- **Interactive art installations** for museums or public spaces  

---

## 🎥 Demo
 

https://github.com/user-attachments/assets/2d0ca31f-c91a-428a-b187-3b79d9365f25
<div align=center>
  <p><strong>Demo 1.</strong> Anemometer</p>
</div>

https://github.com/user-attachments/assets/f828534c-1641-437a-8894-df54421aac83
<div align=center>
  <p><strong>Demo 2.</strong> Xylophone</p>
</div>

https://github.com/user-attachments/assets/cbe92d59-73f2-4cc1-957b-6b1370a6ae0b
<div align=center>
  <p><strong>Demo 3.</strong> Wind Chimes</p>
</div>





---

## 📡 System Overview

- **Sensor Node**: An anemometer measures wind speed and publishes real-time data to an MQTT broker.
- **Actuator Nodes**: Two microcontrollers subscribe to the wind topic and sonify the data:
  - 🎐 **Wind Chime** — Vibration motor strikes metal tubes based on wind intensity.
  - 🎹 **Xylophone** — 8× MG90S servos hit keys mapped to wind levels.

<div align=center>
  <img src="https://raw.githubusercontent.com/JY-SHENNNN/EchoWind/refs/heads/main/Img/mainflow.png" width="800">
  <p><strong>Fig. 1.</strong> System Overview</p>
</div>

---

## ⚙️ Hardware
| Component        | Description                                                                 |
|------------------|-----------------------------------------------------------------------------|
| **Anemometer**   | Wind speed sensor (0–70 m/s), placed in an open-door area                   |
| **Wind Chime**   | Arduino MKR1010 + MOSFET-driven vibration motor                             |
| **Xylophone**    | ESP32-C6 Feather board + 8× MG90S servos                                                   |
| **Power Supply** | Three 5V 2A USB adapters (optional lithium battery backup)                  |

---

## 🔌 Circuit Diagrams
**Sensor Node**  
<div align=center>
    <img src="https://raw.githubusercontent.com/JY-SHENNNN/Dissertation/refs/heads/main/Img/sensorcircuit.png">
    <p><strong>Fig. 2.</strong> Circuit diagram for the sensor node.</p>
</div>

**Wind Chime**  
<div align=center>
    <img src="https://raw.githubusercontent.com/JY-SHENNNN/Dissertation/refs/heads/main/Img/circuit.png" width="800" height="400">
    <p><strong>Fig. 3.</strong> Circuit diagram for the wind chime node.</p>
</div>

**Xylophone**  
<div align=center>
    <img src="https://raw.githubusercontent.com/JY-SHENNNN/Dissertation/refs/heads/main/Img/circuit2.png" width="800" height="400">
    <p><strong>Fig. 4.</strong> Circuit diagram for the xylophone node.</p>
</div>

---

## 🛠 Installation & Setup

### 1️⃣ Hardware Assembly
- **Anemometer:** Connect the anemometer to the Arduino MKR1010 as shown in the diagrams above.
- **Wind Chime:** Attach vibration motor for wind chime to the MKR1010 and connect to the 5V power supply.

- **Xylophone:** Connect the ESP32-C6 Feather board to the 5V power supply and connect the MG90S servos to the ESP32-C6 Add-on board with external power supply.
### 2️⃣ Software Requirements
- Arduino IDE or PlatformIO  
- Libraries:  
  - `PubSubClient.h` (MQTT)  
  - `WiFiNINA.h` (for EMKR1010)  
  - `Adafruit_PWMServoDriver.h` (for MG90S control)  
  - `WiFi.h` (for ESP32-C6)
  - `esp_system.h` (for ESP32-C6)
  - `esp_system.h` (for ESP32-C6)
  - `esp_sleep.h`
  - include `arduino_secrets.h` file for WiFi credentials and MQTT broker details.
### 3️⃣ Code Implementation
The `Code` directory contains the following Arduino sketches: All sketches except `servoCalibrate.ino` require the `arduino_secrets.h` file to be present in the same directory.

- `sensor.ino`  
  Program for the **anemometer node**.

- `Windchime.ino`  
  Program for the **wind chime node**.

- `Glockenspiel.ino`  
  Program for the **xylophone node**.

- `servoCalibrate.ino`  
  Utility sketch for calibrating the **xylophone servos**.


### 4️⃣ Enclosure Design
The `src` directory contains the following 3D-printable STL files:

- `anemometer.stl` and `anemometerbottom.stl`  
  Enclosure parts for the anemometer (can be printed using Fusion 360 or other CAD software).  

- `chimetube.stl`  
  Central wire cover for the wind chime.  

- `holder.stl`  
  Servo holder.  

- `mallet.stl`  
  Xylophone mallet for striking.  

--- 
 ## Package
 ```
1. https://forum.fritzing.org/t/anemometer-in-fritzing/12799/2
2. https://github.com/adafruit/Fritzing-Library
3. https://github.com/johnyHV/fritzing-parts/blob/master/Servo%20MG90S.fzpz
 ```
---
## Reference
https://www.rkonfly.cn/index.php/index/shows/catid/42/id/95.html

