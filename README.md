# Multi-Gas & Air Quality Monitoring System

This project is an Arduino Mega-based **environmental monitoring system** designed to detect and analyze multiple gases, particulate matter, and environmental conditions in real time. It integrates various MQ gas sensors, a particulate matter sensor, temperature/humidity sensing, and GSM communication for alerts.

---

## Project Overview

The system combines multiple sensors to monitor:

- combustible gases  
- toxic gases  
- air quality (PM1.0, PM2.5, PM10)  
- temperature and humidity  
- volatile organic compounds (VOC)  

It is designed for:
- industrial safety monitoring  
- air pollution analysis  
- environmental research  
- smart safety systems  

---

## Features

### 🧪 Multi-Gas Detection

The system uses multiple **MQ sensors** to detect different gases:

| Sensor   | Gas Detected                |
|----------|----------------------------|
| MQ2      | LPG, Smoke                 |
| MQ3      | Alcohol                    |
| MQ4      | Methane (CH₄)              |
| MQ5      | LPG, Natural Gas           |
| MQ6      | LPG                        |
| MQ7      | Carbon Monoxide (CO)       |
| MQ8      | Hydrogen (H₂)              |
| MQ9      | CO, Methane                |
| MQ135    | Air Quality (CO₂, NH₃, etc.) |

---

### 🌫️ Particulate Matter Monitoring (PMS5003)

- Measures:
  - PM1.0  
  - PM2.5  
  - PM10  
- Provides both:
  - standard concentration  
  - environmental concentration  

---

### 🌡️ Temperature & Humidity Monitoring

- Uses **DHT11 sensor**
- Displays:
  - ambient temperature  
  - relative humidity  

---

### 🧴 VOC Detection

- Uses digital pins to classify air quality levels:
  - Clear  
  - Slight Pollution  
  - Medium Pollution  
  - Heavy Pollution  

---

### 📡 GSM Alert System

- Sends SMS alerts using GSM module (Serial1)
- Can notify users when:
  - gas levels exceed thresholds  
  - environmental conditions become unsafe  

---

### 🔁 Continuous Monitoring Cycle

- Sensors run in timed cycles:
  - active reading phase  
  - sampling interval  
- Ensures stable and consistent readings  

---

## System Workflow

### 1. Initialization
- Initializes all MQ sensors  
- Starts serial communication:
  - Serial → debug  
  - Serial1 → GSM  
  - Serial3 → PMS sensor  

---

### 2. Gas Monitoring Cycle
- Continuously reads:
  - MQ sensors (gas concentration)  
  - PMS sensor (particles)  
  - DHT11 (temperature/humidity)  

---

### 3. Data Processing
- Converts analog readings into gas concentration values  
- Processes particulate matter data structure  

---

### 4. Output
- Prints all readings via Serial Monitor  
- Can send alerts via SMS  

---

## Pin Configuration

### Gas Sensors (Analog)

| Sensor   | Pin |
|----------|-----|
| MQ2      | A1  |
| MQ3      | A2  |
| MQ4      | A6  |
| MQ5      | A7  |
| MQ135    | A8  |
| MQ6      | A9  |
| MQ8      | A11 |
| MQ9      | A12 |
| MQ7      | A13 |

---

### Other Sensors

| Component        | Pin |
|------------------|-----|
| DHT11            | 2   |
| Float Sensor     | 3   |
| VOC A Pin        | A5  |
| VOC B Pin        | A4  |

---

### Communication

| Device       | Serial Port |
|--------------|------------|
| GSM Module   | Serial1    |
| PMS5003      | Serial3    |
| Debug Output | Serial     |

---

## Hardware Components

- Arduino Mega 2560  
- MQ Gas Sensors (MQ2–MQ9, MQ135)  
- PMS5003 Dust Sensor  
- DHT11 Temperature & Humidity Sensor  
- GSM Module (SIM800/900)  
- Float Sensor (liquid level detection)  
- Power supply (stable 5V required)  

---

## Key Functions

### Gas Reading
- Uses **MQUnifiedsensor library**
- Reads gas concentration in PPM

---

### PMS Data Reading
- Parses 32-byte data frame from PMS5003  
- Extracts PM values and particle counts  

---

### SMS Sending
```cpp
sendSMS(phoneNumber, message);
