# 🚗 Car Black Box — Automotive Event Data Logger

<div align="center">

![PIC18F4580](https://img.shields.io/badge/MCU-PIC18F4580-blue?style=for-the-badge)
![Language](https://img.shields.io/badge/Language-Embedded%20C-green?style=for-the-badge)
![IDE](https://img.shields.io/badge/IDE-MPLAB%20X-red?style=for-the-badge)
![Compiler](https://img.shields.io/badge/Compiler-XC8-orange?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen?style=for-the-badge)

*An embedded systems project simulating an automotive event data logger using the PIC18F4580 microcontroller.*

</div>

---

## 📖 Overview

The **Car Black Box** is an embedded systems project that simulates an **Automotive Event Data Logger**. Built around the **PIC18F4580** microcontroller, the system continuously monitors critical vehicle events — ignition status, gear changes, collisions, and speed — records them with real-time timestamps, and stores the logs in external EEPROM for future analysis.

A **menu-driven user interface** on a 16x2 Character LCD allows users to view logs, download data via UART, clear records, and configure the Real-Time Clock.

---

## ✨ Features

| Feature | Description |
|---|---|
| 🕒 Real-Time Logging | Records events with accurate date and time |
| 🚀 Speed Monitoring | Acquires vehicle speed via ADC |
| 🔌 RTC Integration | DS1307 for precise timekeeping |
| 💾 EEPROM Storage | Persistent log storage over I²C |
| 📋 View Logs | Displays stored events on Character LCD |
| 📡 Download Logs | Transfers records to PC via UART |
| 🗑️ Clear Logs | Erases all stored event records |
| ⚙️ Set Date & Time | User-configurable RTC settings |
| 🖥️ Menu UI | Interactive menu via Matrix Keypad & LCD |

---

## 🛠️ Hardware Used

- 🧠 **PIC18F4580** Microcontroller
- 🕰️ **DS1307** Real-Time Clock (RTC)
- 💾 **External EEPROM** (I²C)
- 🖥️ **16x2 Character LCD**
- ⌨️ **Matrix Keypad**
- 🔁 **Potentiometer** (Speed Simulation)
- 🔌 **UART Interface**

---

## 💻 Software Used

- 🛠️ **MPLAB X IDE**
- ⚙️ **XC8 Compiler**
- 📝 **Embedded C**

---

## 📡 Technologies & Protocols

| Technology | Usage |
|---|---|
| I²C | EEPROM & RTC communication |
| UART | Log download to PC |
| ADC | Speed acquisition via potentiometer |
| Timers | Event scheduling & timing |
| Interrupts | Real-time event detection |
| EEPROM | Persistent data storage |
| RTC | Timestamping events |

---

## 📁 Project Structure

```
Car-Black-Box/
│
├── 📄 main.c                   # Main application logic
├── 📄 adc.c / adc.h            # ADC driver (speed sensing)
├── 📄 clcd.c / clcd.h          # Character LCD driver
├── 📄 ds1307.c / ds1307.h      # DS1307 RTC driver
├── 📄 external_eeprom.c / .h   # External EEPROM driver
├── 📄 i2c.c / i2c.h            # I²C communication driver
├── 📄 matrix_keypad.c / .h     # Matrix Keypad driver
├── 📄 timer0.c / timer0.h      # Timer0 driver
├── 📄 uart.c / uart.h          # UART communication driver
├── 📁 nbproject/               # MPLAB X project files
└── 📄 README.md
```

---

## ⚙️ Working Principle

```
┌─────────────────────────────────────────────────────────┐
│                    PIC18F4580 Core                       │
│                                                         │
│  ┌──────────┐   I²C   ┌─────────┐  ┌──────────────┐   │
│  │ DS1307   │◄───────►│         │  │ Ext. EEPROM  │   │
│  │   RTC    │         │  MCU    │◄►│  (Log Store) │   │
│  └──────────┘         │         │  └──────────────┘   │
│                       │         │                       │
│  ┌──────────┐  ADC    │         │  ┌──────────────┐   │
│  │   POT    │────────►│         │  │ Character    │   │
│  │ (Speed)  │         │         │◄►│    LCD       │   │
│  └──────────┘         │         │  └──────────────┘   │
│                       │         │                       │
│  ┌──────────┐  GPIO   │         │  ┌──────────────┐   │
│  │  Matrix  │────────►│         │  │    UART      │   │
│  │  Keypad  │         │         │◄►│  (PC Download│   │
│  └──────────┘         └─────────┘  └──────────────┘   │
└─────────────────────────────────────────────────────────┘
```

1. **Event Detection** — The MCU continuously polls sensors and detects vehicle events (ignition, gear, collision, speed).
2. **Timestamping** — The DS1307 RTC provides accurate date and time for each event.
3. **Speed Acquisition** — ADC reads the potentiometer value and maps it to a speed reading.
4. **Storage** — Events are written to External EEPROM over I²C for non-volatile logging.
5. **User Interaction** — Matrix Keypad navigates the LCD menu; options include view, download, clear, and time-set.
6. **Log Download** — Stored records are transmitted to a PC via UART for offline analysis.

---

## 📋 Functionalities

### 📝 Event Logging
- Detects and logs vehicle events in real time
- Captures event timestamp (date & time from DS1307) and current speed
- Persists records in external EEPROM

### 👁️ View Logs
- Scrolls through all stored events on the 16x2 LCD
- Displays event type, timestamp, and speed

### 📡 Download Logs
- Sends all stored logs to a connected PC over UART
- Useful for offline analysis and reporting

### 🗑️ Clear Logs
- Wipes all stored event records from EEPROM
- Confirmed via keypad before deletion

### 🕒 Set Time
- User can configure the current date and time via keypad
- Updates the DS1307 RTC registers directly

---

## 🎓 Learning Outcomes

Working on this project provided hands-on experience with:

- ⚙️ Embedded Firmware Development
- 🔌 Peripheral Interfacing (LCD, Keypad, EEPROM, RTC)
- 📡 I²C & UART Communication Protocols
- 📊 ADC Programming
- ⏱️ Timer & Interrupt-based Design
- 💾 Non-volatile EEPROM Memory Management
- 🖥️ Menu-driven Embedded UI Design
- 🧩 Modular Embedded Programming
- 🐛 Embedded Debugging Techniques

---

## 🚀 Future Enhancements

- [ ] 🚌 CAN Bus integration for real vehicle network communication
- [ ] 📦 SD Card based data logging for higher storage capacity
- [ ] 🛰️ GPS tracking for location-based event records
- [ ] 📱 GSM alert system for remote notifications
- [ ] 🔐 Password-protected menu access
- [ ] 💥 Accident detection using accelerometer/gyroscope sensors

---

## 👨‍💻 Author

**Avinash Patil**

[![GitHub](https://img.shields.io/badge/GitHub-patilavinash0208-181717?style=for-the-badge&logo=github)](https://github.com/patilavinash0208)
[![LinkedIn](https://img.shields.io/badge/LinkedIn-Avinash%20Patil-0A66C2?style=for-the-badge&logo=linkedin)](https://www.linkedin.com/in/avinash-patil-873b9b227)

---

<div align="center">

*If you found this project useful, consider giving it a ⭐ — it helps a lot!*

</div>
