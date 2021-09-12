<p align="center">
  <a href="" rel="noopener">
 <img width=200px height=200px src="Circuit/OEESLogo.png" alt="Project logo"></a>
</p>

<h3 align="center">Operational Equipment Efficieny Sensing</h3>

<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()


</div>

---


<p align="center"> Operational Equipment Efficieny Sensing
    <br> 
</p>

## 📝 Table of Contents

- [About](#about)
- [Getting Started](#getting_started)
- [RPiClient Installation](#Installation)
- [Circuit](#circuit)
- [Usage](#usage)
- [Built Using](#built_using)
- [Authors](#authors)


## 🧐 About <a name = "about"></a>

This repo contains

- Firmware
- Circuit Diagram
- Detailed instructions

for OEESensing project.



## Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your system.


### Prerequisites

Things you need to install the FW.

```
- Arduino IDE
```

### Installing <a name = "installing"></a>

A step by step series that tell you how to get the Firmware and Backend running

#### ESP32 Configuration

You should have Arduino IDE Installed

  1.  Add ESP32 Board to your Arduino IDE
    1. In your Arduino IDE, go to File> Preferences
        Installing ESP32 Add-on in Arduino IDE Windows, Mac OS X, Linux open preferences
    2. Enter ```https://dl.espressif.com/dl/package_esp32_index.json``` 
        into the “Additional Board Manager URLs” field then, click the “OK” button:
        Note: if you already have the ESP32 boards URL, you can separate the URLs with a comma(each board will go to neaw line) as follows:
        ```https://dl.espressif.com/dl/package_esp32_index.json,\n http://arduino.esp8266.com/stable/package_esp8266com_index.json```
    
  2. Open the Boards Manager. Go to Tools > Board > Boards Manager…
  3. Search for ESP32 and press install button for the ESP32 by Espressif Systems“:
  4. That’s it. It should be installed after a few seconds.

  5.  Now copy the contents of the libs folder to the libraries directory of your Arduino
      1. If you are using windows, the libraries directory will be Documents/Arduino/libraries

##### ESP32 Node FW Uploading
  1.  Select ESP32 Dev Module from Tools->Board->ESP32
  2.  Select the correct port from Tools->Port
  3.  Then open Firmware.ino file,
  4.  Upload the Code to your ESP32 Dev Module.
  5.  Your ESP32 is now ready to be used.



## Circuit <a name = "circuit"></a>


### ESP32 Dev Module Pinout


Follow the pinout diagram given below to connect different components to your TTGO LORA32 board.

![LoraPinout](Circuit/ESP32-Pinout.jpg)

### Other Components

```http
Other components pin connection details
```


#### Temperature Sensor DHT22

```DHT22 Connections```

| DHT22 Pins | ESP32 Dev Module Pins| 
| :--- | :--- | 
| `DATA OUT` | `23` |
| `VCC` | `5V` |
| `GND` | `GND` | 

#### Vibration Sensor(MPU6050)

```MPU6050 Connections```

| MPU6050 Pins | ESP32 Dev Module | 
| :--- | :--- | 
| `SCL` | `22` |
| `SDA` | `21` |
| `VCC` | `3.3V` |
| `GND` | `GND` | 

#### Status LED

```LED Connections```

| LED Pins | ESP32 Dev Module | 
| :--- | :--- | 
| `Anode` | `35 via 220Ω resistor` |
| `Cathode` | `GND` |

#### SCT-013 

```SCT-013 Connections```

-   Voltage Divider with 2x 100KΩ resistors.
-   10uF capacitor connected between Voltage Divider Circuit Output and GND
-   3.5mm Audio Jack connected between Voltage Divider Circuit Output and ESP32 Pin 34.

The overall SCT-013 connection assembly will look something like shown in the diagram below.

![SCTConnection](Circuit/sctWiring.jpg)

### Complete Circuit Diagram

Here's the complete circuit diagram of the system.

![CircuitDiagram](Circuit/Circuit_bb.png)

## Usage <a name = "usage"></a>

<!-- 1.  Run installer script on your Raspberry Pi.
2.  Setup the camera as mentioned in the ``Configuring Camera`` section. -->


## List of Components <a name = "list"></a>

Following components are used to make this project

1.  ESP32 Dev Kit Module
https://www.amazon.com/HiLetgo-ESP-WROOM-32-Development-Microcontroller-Integrated/dp/B0718T232Z/ref=sr_1_3?crid=5EOAXOANUSCU&dchild=1&keywords=esp32+nodemcu&qid=1629587138&sprefix=esp32+node%2Caps%2C201&sr=8-3

2.  Current Sensor (SCT-013)
https://www.amazon.com/dp/B083S6YG36/ref=sspa_dk_detail_2?psc=1&pd_rd_i=B083S6YG36&pd_rd_w=lyRRH&pf_rd_p=887084a2-5c34-4113-a4f8-b7947847c308&pd_rd_wg=ARQkk&pf_rd_r=9X6KNTP47QE6X1QG8RGX&pd_rd_r=92990704-e6cd-4c90-9b74-93726cb99938&smid=A1G4TRJSF885ET&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUFWVzdCSUhXNFU2VEwmZW5jcnlwdGVkSWQ9QTAxODU2MTcxQjNFVUI5M1ZTSUEwJmVuY3J5cHRlZEFkSWQ9QTA3OTA4NTcxR0ZGQ1JOQUFHSFFPJndpZGdldE5hbWU9c3BfZGV0YWlsJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==

3.  DHT22 Temperature and Humidity Sensor
https://www.amazon.com/Gowoops-Temperature-Humidity-Measurement-Raspberry/dp/B073F472JL/ref=sr_1_7?dchild=1&keywords=dht22&qid=1624855583&sr=8-7

4.  Vibration Sensor (IMU) (MPU6050)
https://www.amazon.com/HiLetgo-MPU-6050-Accelerometer-Gyroscope-Converter/dp/B00LP25V1A/ref=sr_1_1?dchild=1&keywords=mpu6050&qid=1624855642&sr=8-1

5.  Generic 3.5mm Green LED
6.  Generic 3.5mm Audio Jack
7.  Generic 10uF 25v Capacitor
8.  Generic 220Ω and 100KΩ resistors
9.  Generic 5V USB Micro B cable and adapter.

## ⛏️ Built Using <a name = "built_using"></a>

- [Python](https://www.python.org/) - For Cloud Gateway Pogramming
- [Arduino](https://www.arduino.cc/) - Embedded Framework and IDE - For Sensor Node Design
  

## ✍️ Authors <a name = "authors"></a>

- [@Nauman3S](https://github.com/Nauman3S) - Development and Deployment
