# EchoWind: A Connected System for Wind Sensin and Sonified Feedback

This project explores how real-time wind data can be translated into sound through two physical, interactive devices designed for indoor environments — a vibration-driven wind chime and a servo-controlled xylophone. The system aims to enhance environmental perception and foster biophilic interaction in architectural spaces.
<div align=center>
    <img src="https://raw.githubusercontent.com/JY-SHENNNN/Dissertation/refs/heads/main/Img/device.jpg" width="800" height="400">
</div>


https://github.com/user-attachments/assets/5dc4c54e-c8f3-4238-8c74-f22c6738b43d


https://github.com/user-attachments/assets/465d06ca-0d83-4de3-b87b-9f1a49d4c236



---

## 📡 System Overview

- **Sensor Node**: An anemometer reads wind speed and publishes real-time values to an MQTT broker.
- **Actuator Nodes**: Two microcontrollers subscribe to the wind topic and sonify the data:
  - 🎐 **Wind Chime**: Uses a vibration motor to activate metal tubes based on wind intensity.
  - 🎹 **Xylophone**: Uses 8 MG90S servos to strike keys mapped to wind levels.

<div align=center>
  <img src="https://raw.githubusercontent.com/JY-SHENNNN/Dissertation/refs/heads/main/Img/mainflow.png" width="800">
  <p><strong>Fig. 1.</strong> System overview</p>
</div>

---

## ⚙️ Hardware
| Component        | Description                                                                 |
|------------------|-----------------------------------------------------------------------------|
| **Anemometer**   | Wind speed sensor (0-70 m/s), placed near in a opendoor area              |
| **Wind Chime**   | Controlled by Arduino MKR1010, powered via MOSFET-driven vibration motor    |
| **Xylophone**    | Controlled by ESP32-C6 with 8x MG90S servos           |
| **Power Supply** | Three 5V 2A USB power adapters, optional lithium battery backup             |

---

## 🔌 Connections
circuit diagram for anemometer:
<div align=center>
    <img src="https://raw.githubusercontent.com/JY-SHENNNN/Dissertation/refs/heads/main/Img/sensorcircuit.png">
 </div>   

circuit diagram for windchime:
<div align=center>
    <img src="https://raw.githubusercontent.com/JY-SHENNNN/Dissertation/refs/heads/main/Img/circuit.png" width="800" height="400">
</div>
circuit diagram for xylophone:
<div align=center>
    <img src="https://raw.githubusercontent.com/JY-SHENNNN/Dissertation/refs/heads/main/Img/circuit2.png" width="800" height="400">
 </div>

 ---

 ## Package
 ```
1. https://forum.fritzing.org/t/anemometer-in-fritzing/12799/2
2. https://github.com/adafruit/Fritzing-Library
3. https://github.com/johnyHV/fritzing-parts/blob/master/Servo%20MG90S.fzpz
 ```
