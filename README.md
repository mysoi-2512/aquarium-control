# 🐠 Aquarium Control System

## Please view Report [HERE](https://docs.google.com/document/d/1OF4tAau-0sanhd8cBZsHKmiU7_impTn0-SaQfgHeYFg/edit?usp=sharing)

## 📌 Project Description
An embedded project using ATmega328P to control aquarium. It features IR remote control for LEDs (decoration) and servo (feeding), temperature sensing with DS18B20, PWM-driven cooling with pump and thermoelectric cooler, and LCD display for real-time status.

---
## 👥 Team Members
| My | Merge code, PWM pump + cooler, Proteus, Altium |

| Hoang | Temperature sensor, LCD display, Arduino Interfacing, Material Bill |

| Khanh | IR remote, LED chasing, Servo motor |

---
## 📌 Function Description

This embedded project is divided into the following functional modules:

### 1. IR Remote Control Module
- Receives IR signals from a remote using IR1838.
- Supports 5 buttons:
  - Turn ON all LEDs
  - Turn OFF all LEDs
  - Two LED chasing modes
  - Control servo motor angle

### 2. LED Chasing Module
- Controls a 16-LED bar based on commands from the IR module.
- Supports:
  - All LEDs ON/OFF
  - Two different chasing light patterns.

### 3. Servo Motor Control
- Rotates servo based on remote control input.
- Be used for feeding fish.

### 4. Temperature Monitoring (DS18B20)
- Continuously reads water temperature using DS18B20 sensor.
- Sends temperature data to LCD for display.
- Provides temperature info to PWM module for cooling control.

### 5. PWM Cooling Control (Pump + TEC1)
- Uses PWM signal to control a transistor driving the water pump.
- Activates the TEC1 cooler (via relay) when pump is ON.
- PWM duty cycle depends on water temperature:
  - Gradually increases if temperature > 29°C
  - Stops if temperature ≤ 27°C
- Status LED:
  - Red when pump is active
  - Green when pump is off

### 6. LCD Display + I2C Module
- Displays:
  - Current water temperature
  - Pump status (ON/OFF)
  - PWM duty cycle (optional)


