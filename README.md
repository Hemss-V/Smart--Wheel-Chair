# Autonomous Smart Wheelchair

## Overview
The Autonomous Smart Wheelchair is an embedded robotics system designed to enhance mobility and safety through real-time obstacle detection and assisted navigation.

The system integrates sensor-based perception, embedded control, and actuation to enable safe movement in dynamic environments.

---

## Features
- Real-time obstacle detection using ultrasonic sensors  
- Automatic collision avoidance (stop on obstacle detection)  
- Voice-based control using Bluetooth (HC-05)  
- Manual control mode for flexibility  
- Motor control using L298N driver  

---

## Tech Stack
- **Microcontroller:** Arduino Uno  
- **Programming:** C++  
- **Sensors:** Ultrasonic Sensor  
- **Communication:** Bluetooth (HC-05)  
- **Motor Driver:** L298N  

---

## System Architecture
Ultrasonic Sensor → Arduino (Processing & Control Logic) → Motor Driver (L298N) → Motors

---

## How It Works
1. Ultrasonic sensor continuously measures distance  
2. Arduino processes sensor data in real time  
3. If an obstacle is detected within threshold:
   - Movement is stopped automatically  
4. User can control movement via:
   - Voice commands (Bluetooth)
   - Manual input  

---

## Demo
Video: https://drive.google.com/file/d/1F6YnalWSdtnnSfoC2tOtHRAJ-bEUTNyT/view

---

## Future Improvements
- Integration with ROS for modular control  
- Advanced obstacle avoidance (path planning)  
- Sensor fusion (LiDAR / camera)  
- Semi-autonomous navigation  

---

## Author
Hema Varshini
