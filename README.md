# FlameGuard

A smart fire detection system using Arduino Uno that monitors temperature and light intensity via DHT22 and LDR sensors, triggering buzzer and LED alerts on fire hazard detection.

Developed and tested on Wokwi online simulator.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Components](#hardware-components)
- [Pin Connections](#pin-connections)
- [Installation](#installation)
- [Usage](#usage)
- [Detection Thresholds](#detection-thresholds)
- [Wokwi Simulation](#wokwi-simulation)
- [Troubleshooting](#troubleshooting)
- [Project Notes](#project-notes)

---

## Overview

FlameGuard is an Arduino Uno based fire alert system that continuously monitors ambient temperature using a DHT22 sensor and detects sudden light intensity changes via an LDR photoresistor module. When a fire hazard is detected, the system triggers an audible buzzer alarm, activates a red LED, and displays a live alert on a 16x2 I2C LCD screen. The system automatically resets to safe state once the hazard clears.

---

## Features

- Real-time temperature and humidity monitoring via DHT22 sensor
- Light intensity based fire detection via LDR photoresistor module
- Audible alert using active buzzer with tone() function
- Visual alert using red LED on fire detection
- Green LED for safe status indication
- 16x2 I2C LCD display showing live sensor readings and system status
- Serial monitor output for full data logging and debugging
- Automatic reset to safe state when hazard clears

---

## Hardware Components

| Component              | Quantity | Purpose                              |
|------------------------|----------|--------------------------------------|
| Arduino Uno            | 1        | Main microcontroller                 |
| DHT22 Sensor           | 1        | Temperature and humidity measurement |
| LDR Photoresistor Module | 1      | Light intensity / fire glow detection|
| Active Buzzer          | 1        | Audible fire alert                   |
| Red LED                | 1        | Fire alert indicator                 |
| Green LED              | 1        | Safe status indicator                |
| 220 Ohm Resistor       | 2        | Current limiting for LEDs            |
| 16x2 LCD (I2C)         | 1        | Display readings and system status   |
| Jumper Wires           | As needed| Component connections                |
| Breadboard             | 1        | Prototyping connections              |

---

## Pin Connections

### DHT22 Sensor

| DHT22 Pin | Arduino Pin    | Notes             |
|-----------|----------------|-------------------|
| VCC       | 5V             | Power supply      |
| SDA       | Digital Pin 2  | Data line         |
| NC        | Not connected  | Leave empty       |
| GND       | GND            | Ground            |

### LDR Photoresistor Module

| LDR Pin | Arduino Pin    | Notes                          |
|---------|----------------|--------------------------------|
| VCC     | 5V             | Power supply                   |
| GND     | GND            | Ground                         |
| AO      | Analog Pin A0  | Analog output used for readings|
| DO      | Not connected  | Not used in this project       |

### Active Buzzer

| Buzzer Pin        | Arduino Pin   | Notes                        |
|-------------------|---------------|------------------------------|
| Pin 1 (negative)  | GND           | Ground                       |
| Pin 2 (positive)  | Digital Pin 8 | Controlled via tone() function|

### LEDs

| Component           | Arduino Pin    | Resistor          |
|---------------------|----------------|-------------------|
| Red LED Anode (+)   | Digital Pin 9  | 220 Ohm in series |
| Red LED Cathode (-) | GND            | -                 |
| Green LED Anode (+) | Digital Pin 10 | 220 Ohm in series |
| Green LED Cathode (-)| GND           | -                 |

### LCD I2C (16x2)

| LCD Pin | Arduino Pin    |
|---------|----------------|
| VCC     | 5V             |
| GND     | GND            |
| SDA     | Analog Pin A4  |
| SCL     | Analog Pin A5  |

---

## Installation

1. Clone this repository

```bash
git clone https://github.com/your-username/flameguard-arduino.git
```

2. Open `sketch.ino` in Arduino IDE

3. Install required libraries via Sketch > Include Library > Manage Libraries

| Library Name           | Author    |
|------------------------|-----------|
| DHT sensor library     | Adafruit  |
| Adafruit Unified Sensor| Adafruit  |
| LiquidCrystal I2C      | Frank de Brabander |

4. Connect components as per the pin connections table above

5. Upload the sketch to your Arduino Uno

---

## Usage

Once uploaded and powered on, the system will:

- Display an initialization message on the LCD for 2 seconds
- Begin monitoring temperature and light intensity
- Show live readings on the LCD and serial monitor
- Trigger fire alert when either sensor crosses its threshold
- Automatically return to safe state when readings normalize

### Serial Monitor Output

Set baud rate to 9600 in the serial monitor.

Normal state:
```
=== Fire Alert System Started ===
Monitoring temperature and light...
Temp: 28.00 C  |  Humidity: 60.00 %  |  LDR: 650  |  Status: Safe
```

Fire detected:
```
Temp: 56.40 C  |  Humidity: 40.00 %  |  LDR: 315  |  Status: *** FIRE DETECTED ***
>>> ALERT: Fire hazard detected! Take action! <<<
```

---

## Detection Thresholds

| Parameter         | Default Value | Description                                      |
|-------------------|---------------|--------------------------------------------------|
| TEMP_THRESHOLD    | 50.0 C        | Fire alert triggers above this temperature       |
| LDR_THRESHOLD     | 300           | Fire alert triggers when LDR reads below this    |
| BUZZER_BEEP_DELAY | 300 ms        | Interval between buzzer beeps                    |

These values can be adjusted at the top of `sketch.ino` to suit your environment.

```cpp
#define TEMP_THRESHOLD     50.0   // Change to 35.0 for easy testing
#define LDR_THRESHOLD      300    // Increase to 500 if LDR is too sensitive
#define BUZZER_BEEP_DELAY  300    // ms between beeps
```

### Alert Output States

| Output       | Fire Detected    | Safe State      |
|--------------|------------------|-----------------|
| Red LED      | ON               | OFF             |
| Green LED    | OFF              | ON              |
| Buzzer       | Beeping 1000 Hz  | Silent          |
| LCD Row 1    | T:XX.XC H:XX%    | T:XX.XC H:XX%  |
| LCD Row 2    | !! FIRE ALERT !! | Status:  SAFE  |
| Serial Monitor| FIRE DETECTED   | Safe            |

---

## Wokwi Simulation

This project was built and tested on Wokwi. To run the simulation:

1. Open [Wokwi](https://wokwi.com) and create a new Arduino Uno project
2. Copy `sketch.ino` into the editor
3. Copy `diagram.json` into the diagram tab
4. Copy library names into `libraries.txt`
5. Click Play to start

### Testing DHT22 in Wokwi

- Click on the DHT22 component during simulation
- A slider popup appears
- Drag the temperature slider above 50 degrees Celsius
- Fire alert triggers immediately
- Drag the slider back below 50 degrees to reset

### Testing LDR in Wokwi

- Click on the LDR component during simulation
- A brightness slider appears
- Drag slider toward the bright side
- When LDR value drops below 300 in serial monitor, alert triggers
- Drag slider back to reduce brightness to reset

---

## Troubleshooting

| Problem                  | Likely Cause                        | Solution                                      |
|--------------------------|-------------------------------------|-----------------------------------------------|
| LCD shows nothing        | Wrong I2C address                   | Change 0x27 to 0x3F in code                   |
| DHT22 read failed error  | Loose or wrong wiring               | Verify SDA connected to Digital Pin 2         |
| Buzzer not sounding      | Using digitalWrite instead of tone()| Use tone(8, 1000, 200) in code                |
| LDR never triggers alert | Threshold too low                   | Lower LDR_THRESHOLD from 300 to 500           |
| LDR always triggering    | Bright ambient light                | Raise LDR_THRESHOLD to 200                    |
| LCD shows Sensor Error   | DHT22 not initialized               | Add delay(2000) after dht.begin()             |
| No serial output         | Wrong baud rate                     | Set serial monitor to 9600 baud               |

---

## Project Notes

- The LDR photoresistor module used in Wokwi has a built-in pull-down resistor. No external resistor is needed.
- The DHT22 sensor in Wokwi does not require a pull-up resistor in simulation. Connect SDA directly to Digital Pin 2.
- The buzzer must use the tone() function in Wokwi simulation. The digitalWrite() method does not produce audible sound in the simulator.
- The LCD I2C address is 0x27 by default. If the display remains blank, try 0x3F.
- The initialization message is limited to 16 characters per row on the LCD.

---

## License

This project is open source and available under the [MIT License](LICENSE).