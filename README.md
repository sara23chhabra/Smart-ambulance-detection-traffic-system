# Smart Ambulance Traffic Control System

An IoT-based intelligent traffic control system that automatically gives priority to ambulances at traffic intersections using **RFID detection and computer vision**.

The system integrates **Arduino hardware control** with a **Python-based AI ambulance detection model** to dynamically change traffic signals and reduce emergency response time.

---

## Features

- Real-time **ambulance detection using computer vision**
- **RFID authentication** for ambulance vehicles
- Automatic **traffic signal priority switching**
- Integration between **Python AI detection and Arduino traffic control**
- Multi-road traffic signal management (3 intersections)

---

## System Architecture
Ambulance
↓
RFID Tag Detection / Camera Detection
↓
Python Computer Vision Model (Roboflow)
↓
Serial Communication
↓
Arduino Traffic Controller
↓
Traffic Light Priority Activation


---

## Technologies Used

### Hardware
- Arduino
- MFRC522 RFID Readers
- Traffic Light LEDs
- Serial Communication

### Software
- Python
- OpenCV
- Roboflow Object Detection
- PySerial
- Arduino C++

---

## Project Structure
smart-ambulance-traffic-system
│
├── arduino
│ └── ambulance_detection.ino
│
├── python
│ └── arduino_ambulance.py
│
├── requirements.txt
└── README.md


---

## Installation

### 1 Clone the Repository
git clone https://github.com/YOUR\_USERNAME/smart-ambulance-traffic-system.git
cd smart-ambulance-traffic-system

---

### 2 Install Python Dependencies
pip install -r requirements.txt

---

### 3 Upload Arduino Code

Open Arduino IDE and upload:
arduino/traffic_control.ino

to your Arduino board.

---

### 4 Run the Python Detection Script
cd python
python ambulance_detection.py

The system will start monitoring the webcam for ambulance detection.

---

## How It Works

1. The Python script captures live webcam frames.
2. The Roboflow object detection model identifies ambulances.
3. When an ambulance is detected:
   - A signal number is sent via serial communication.
4. Arduino receives the signal and activates the corresponding green light.
5. RFID readers also allow authenticated ambulance tags to trigger priority.

---

## Future Improvements

- Multi-camera traffic monitoring
- Cloud-based traffic management
- Automatic ambulance GPS tracking
- Smart city traffic integration

---

## Author

Sara Chhabra
