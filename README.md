# IoT-based-Blackout-Detecor-Fan
Project for the CEN4100 - Internet of Things course
---
📌 Overview
---
This project implements a smart, energy-aware fan system that operates based on real-time room temperature and household electricity supply conditions.
The system is designed to automatically activate cooling during power fluctuations or blackouts, providing an energy-efficient and convenient solution for households with unreliable electrical grids.

🎯 Motivation
---
Traditional air-conditioning systems consume large amounts of energy and often operate inefficiently.
Fans offer a significantly more energy-efficient alternative, especially in regions where power outages and unstable electricity supply are common.

This project aims to enhance conventional fan systems by introducing automation, intelligence, and energy awareness, reducing unnecessary electricity usage while improving user comfort.

🧠 System Architecture
---
The system follows a 3-tier IoT architecture:

Hardware Layer:
ESP32 microcontroller, LM35 temperature sensor, and a servo motor used to simulate fan control.

Network Layer:
Bluetooth Low Energy (BLE) for communication between the ESP32 and the application layer.

Application Layer:
nRF Connect for monitoring sensor data and system interaction.

▶️ How to Run
---
🔧 Hardware Requirements

ESP32 microcontroller

LM35 temperature sensor

Servo motor

Breadboard and jumper wires

📷 Hardware setup:


🛠️ Software Requirements

Arduino IDE

ESP32 board support package

nRF Connect mobile application

🚀 Steps
---
Connect the hardware components as shown in the diagram.

Upload the ESP32 firmware using the Arduino IDE.

Power the ESP32 and connect via Bluetooth Low Energy (BLE) using nRF Connect.

Monitor temperature and electricity conditions in real time.

The fan automatically turns ON or OFF based on system logic.
