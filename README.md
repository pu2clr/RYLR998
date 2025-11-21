
# RYLR998

This project provides a comprehensive, step-by-step tutorial for developing IoT applications using the RYLR998 LoRa® transceiver module from REYAX Technology. Designed for hobbyists, IoT enthusiasts, electronics technicians, and educational robotics practitioners, this guide covers everything from initial setup and configuration to advanced application development. Whether you're building long-range sensor networks, exploring LoRa technology for the first time, or implementing professional IoT solutions, this repository offers practical examples, detailed documentation, and ready-to-use code for popular microcontroller platforms like ESP8266, LGT8F328P, and Raspberry Pi Pico (MicroPython).


## About RYLR998

The RYLR998, developed by REYAX Technology, is a compact LoRa® transceiver module designed for ultra-long-range communication, high interference resistance, and low power consumption. Operating in the 868/915 MHz frequency range, it integrates a NUVOTON MCU and Semtech LoRa® engine, supports UART with AT commands, and achieves transmission distances of up to 20 km in ideal conditions—all within a small 32×14×2.2 mm form factor suitable for space-limited applications.
Compared to traditional wireless methods like FSK or GFSK, the RYLR998 offers higher sensitivity (down to -129 dBm) and extended battery life, making it ideal for IoT devices that rely on efficient energy use. Despite its slightly higher cost, its superior range, reliability, and efficiency make it a cost-effective alternative for markets such as smart homes, agriculture, vehicles, mobile devices, and energy monitoring, helping enterprises build IoT solutions more effectively. See [REYAX new LoRa® Antenna Transceiver Module RYLR998](https://reyax.com/news_d.php?nw=3)


## Project Structure

This repository is organized to help developers quickly start working with the RYLR998 module. The project includes:

```
RYLR998/
├── README.md                    # Main project documentation
├── examples/                    # Code examples for different platforms
│   ├── ESP8266/                # Examples using ESP8266 (NodeMCU, Wemos D1, etc.)
│   ├── LGT8F328P/              # Examples using LGT8F328P (3.3V Arduino Nano compatible)
│   └── RaspberryPi_Pico/       # Examples using Raspberry Pi Pico (MicroPython)
├── docs/                       # Additional documentation and guides
├── images/                     # Photos and diagrams
└── schematics/                 # Circuit diagrams and connection schematics
    └── kicad/                  # KiCAD project files (.kicad_pro, .kicad_sch, .kicad_pcb)
```

### About Schematics and Diagrams

All circuit schematics in this project are created using **KiCAD** (version 7.0 or higher), a free and open-source electronics design automation (EDA) suite. KiCAD files are provided in the `schematics/kicad/` directory, allowing you to:

- View and modify circuit designs
- Generate your own PCB layouts
- Export schematics to PDF or image formats
- Customize connections for your specific needs

**To use the KiCAD files:**
1. Download and install [KiCAD](https://www.kicad.org/download/) (free and cross-platform)
2. Open the `.kicad_pro` project files from the `schematics/kicad/` folder
3. View schematics (`.kicad_sch`) and PCB layouts (`.kicad_pcb`) if available
4. Export to your preferred format (PDF, PNG, SVG, etc.)

Pre-rendered schematic images are also available in the `images/` folder for quick reference.


## About the Device RYLR998

### Technical Specifications

| Specification | Value |
|--------------|-------|
| **Frequency Range** | 868 MHz / 915 MHz |
| **Operating Voltage** | 3.3V (2.0V - 3.6V) |
| **Transmission Power** | Up to +20 dBm |
| **Receive Sensitivity** | Down to -148 dBm |
| **Communication Distance** | Up to 20 km (open field) |
| **Interface** | UART (3.3V TTL) |
| **Baud Rate** | 115200 bps (default) |
| **Dimensions** | 32 × 14 × 2.2 mm |
| **Control Method** | AT Commands |
| **Modulation** | LoRa® Spread Spectrum |

### Key Features

- **Ultra-Low Power Consumption**: Ideal for battery-powered applications
- **Long-Range Communication**: Reaches up to 20 km in ideal conditions
- **High Interference Immunity**: LoRa® technology provides excellent resistance to interference
- **Simple Integration**: UART interface with AT commands for easy configuration
- **Compact Design**: Small form factor suitable for space-constrained projects
- **3.3V Operation**: Directly compatible with modern microcontrollers


## Getting Started

### Hardware Requirements

To work with the RYLR998 module, you will need:

1. **RYLR998 Module** (at least 2 units for testing communication)
2. **Microcontroller** (choose one):
   - ESP8266 (NodeMCU, Wemos D1 Mini, etc.)
   - LGT8F328P (3.3V Arduino Nano compatible)
   - Raspberry Pi Pico (MicroPython)
   - Any other 3.3V microcontroller with UART
3. **USB-to-TTL Converter** (for initial testing and configuration)
4. **Breadboard and Jumper Wires**
5. **Power Supply** (3.3V regulated)

**Optional Tools:**
- **KiCAD** (for viewing/editing circuit schematics) - [Download here](https://www.kicad.org/download/)

### Why Not Arduino UNO or Nano?

You may notice that this project uses the **LGT8F328P** instead of the traditional Arduino UNO or Nano. Here's why:

**The Problem with Standard Arduino UNO/Nano:**
- Arduino UNO and Nano operate at **5V logic levels**
- The RYLR998 module operates at **3.3V** and is **not 5V tolerant**
- Direct connection between 5V Arduino and RYLR998 can **damage the module**
- Using 5V Arduino requires additional components:
  - Voltage level shifters (bidirectional logic level converter)
  - Voltage divider circuits for each signal line
  - Additional wiring and complexity
  - More space on breadboard
  - Higher cost for additional components

**The LGT8F328P Solution:**
- **LGT8F328P** is a cost-effective alternative that runs at **3.3V natively**
- **100% Arduino Nano compatible** (same code, same IDE, same libraries)
- **Direct connection** to RYLR998 without any level shifting
- **Simplified wiring** and fewer components
- **Lower project cost** (no need for level shifters)
- **More reliable** (fewer connections = fewer potential failures)

**Can you still use Arduino UNO/Nano?**

Yes, you can! But you'll need to add level shifting circuitry between the Arduino (5V) and RYLR998 (3.3V). While this is certainly possible, it adds complexity that this tutorial aims to avoid. By using native 3.3V microcontrollers (ESP8266, LGT8F328P, Raspberry Pi Pico), we keep the projects simple, safe, and beginner-friendly.

### Important Notes

⚠️ **Voltage Level**: The RYLR998 operates at **3.3V**. Do not connect it directly to 5V microcontrollers (like standard Arduino Uno) without level shifting.

✅ **Compatible Platforms**:
- ESP8266 (native 3.3V)
- LGT8F328P (3.3V operation)
- Raspberry Pi Pico (native 3.3V, MicroPython support)
- ESP32 (native 3.3V)
- STM32 (3.3V variants)
- Any 3.3V microcontroller with UART

### Basic Connections

The RYLR998 has the following pins:

| Pin | Function | Description |
|-----|----------|-------------|
| VDD | Power Supply | 3.3V power input (2.0V - 3.6V) |
| GND | Ground | Ground connection |
| RXD | UART Receive | Connect to MCU TX (3.3V logic) |
| TXD | UART Transmit | Connect to MCU RX (3.3V logic) |
| RST | Reset | Active low reset (optional) |


## Initial Setup and Testing

### Step 1: Test with USB-to-TTL Converter

Before integrating with a microcontroller, it's recommended to test the module using a USB-to-TTL converter and a serial terminal.

1. Connect the RYLR998 to the USB-to-TTL converter:
   - RYLR998 VDD → 3.3V
   - RYLR998 GND → GND
   - RYLR998 RXD → TX
   - RYLR998 TXD → RX

2. Open a serial terminal (Arduino IDE Serial Monitor, PuTTY, etc.)
3. Set baud rate to **115200**
4. Test with basic AT commands:
   ```
   AT
   AT+VER?
   AT+ADDRESS?
   AT+NETWORKID?
   ```

### Step 2: Basic AT Commands

| Command | Description | Example |
|---------|-------------|---------|
| `AT` | Test communication | Returns `+OK` |
| `AT+VER?` | Get firmware version | Returns version info |
| `AT+ADDRESS?` | Get device address | Returns current address |
| `AT+ADDRESS=<addr>` | Set device address (0-65535) | `AT+ADDRESS=100` |
| `AT+NETWORKID?` | Get network ID | Returns network ID |
| `AT+NETWORKID=<id>` | Set network ID (3-15) | `AT+NETWORKID=6` |
| `AT+PARAMETER?` | Get LoRa parameters | Returns spread factor, bandwidth, etc. |
| `AT+SEND=<addr>,<len>,<data>` | Send data | `AT+SEND=100,5,Hello` |

### Step 3: Configure Two Modules for Communication

To establish communication between two RYLR998 modules:

**Module 1 (Transmitter):**
```
AT+ADDRESS=100
AT+NETWORKID=6
```

**Module 2 (Receiver):**
```
AT+ADDRESS=200
AT+NETWORKID=6
```

**Send test message from Module 1:**
```
AT+SEND=200,11,Hello World
```

**Module 2 should receive:**
```
+RCV=100,11,Hello World,-30,50
```
Format: `+RCV=<sender_address>,<length>,<data>,<RSSI>,<SNR>`


## Using COMFORT Software (Reyax Tool)

COMFORT is a Windows-based software tool developed by Reyax specifically designed for monitoring, testing, and debugging LoRa applications using their modules, including the RYLR998.

### About COMFORT

COMFORT provides a graphical interface for:
- Real-time monitoring of LoRa communication
- Easy configuration of module parameters
- Testing and debugging LoRa networks
- Visualizing signal strength (RSSI) and quality (SNR)
- Logging communication data

### Key Features

- **User-Friendly Interface**: Graphical environment for AT command interaction
- **Real-Time Monitoring**: Live view of transmitted and received messages
- **Parameter Configuration**: Easy setup of addresses, network IDs, and LoRa parameters
- **Signal Analysis**: RSSI and SNR visualization for link quality assessment
- **Data Logging**: Record communication sessions for analysis
- **Multi-Module Support**: Manage multiple RYLR998 modules simultaneously

### Getting Started with COMFORT

1. **Download**: Obtain COMFORT software from Reyax official website or support channels
2. **Installation**: Install on Windows PC
3. **Connection**: Connect RYLR998 module via USB-to-TTL converter
4. **Configuration**: Select appropriate COM port and baud rate (115200)
5. **Testing**: Use the interface to send AT commands and monitor responses

### When to Use COMFORT

COMFORT is particularly useful for:
- Initial module testing and verification
- Network configuration and optimization
- Range testing and signal quality analysis
- Troubleshooting communication issues
- Learning LoRa communication concepts
- Prototyping before firmware development

> **Note**: This section will be expanded with detailed instructions, screenshots, and usage examples as the software evaluation progresses.


## Development with ESP8266

The ESP8266 is an excellent choice for RYLR998 projects due to its native 3.3V operation, WiFi capability, and Arduino IDE support.

### Wiring Diagram (ESP8266)

| RYLR998 Pin | ESP8266 Pin | Notes |
|-------------|-------------|-------|
| VDD | 3.3V | Power |
| GND | GND | Ground |
| RXD | D7 (GPIO13) | Software Serial TX |
| TXD | D6 (GPIO12) | Software Serial RX |

See the `examples/ESP8266/` folder for complete working examples.


## Development with LGT8F328P

The LGT8F328P is an Arduino Nano-compatible microcontroller that operates at 3.3V, making it ideal for direct connection to the RYLR998 without level shifters.

### Wiring Diagram (LGT8F328P)

| RYLR998 Pin | LGT8F328P Pin | Notes |
|-------------|---------------|-------|
| VDD | 3.3V | Power |
| GND | GND | Ground |
| RXD | D3 (TX) | Software Serial or Hardware Serial |
| TXD | D2 (RX) | Software Serial or Hardware Serial |

See the `examples/LGT8F328P/` folder for complete working examples.


## Development with Raspberry Pi Pico (MicroPython)

The Raspberry Pi Pico is an excellent platform for RYLR998 projects, featuring native 3.3V operation, dual-core ARM Cortex-M0+ processor, and excellent MicroPython support.

### Wiring Diagram (Raspberry Pi Pico)

| RYLR998 Pin | Pico Pin | GPIO | Notes |
|-------------|----------|------|-------|
| VDD | 3.3V (Pin 36) | - | Power |
| GND | GND (Pin 38) | - | Ground |
| RXD | GP4 (Pin 6) | UART1 TX | Software or Hardware Serial TX |
| TXD | GP5 (Pin 7) | UART1 RX | Software or Hardware Serial RX |

### MicroPython Setup

1. **Install MicroPython**: Flash MicroPython firmware to your Raspberry Pi Pico ([download here](https://micropython.org/download/rp2-pico/))
2. **IDE Options**:
   - Thonny (recommended for beginners)
   - Visual Studio Code with Pico-W-Go extension
   - uPyCraft
3. **UART Configuration**: MicroPython makes UART communication simple with built-in libraries

### Why Raspberry Pi Pico?

- **Native 3.3V**: Direct connection to RYLR998 without level shifters
- **MicroPython**: Easy-to-learn Python syntax, ideal for rapid prototyping
- **Multiple UART**: Two hardware UART interfaces available
- **Low Cost**: Affordable platform for learning and prototyping
- **Rich Ecosystem**: Extensive library support and community resources
- **Educational**: Perfect for teaching IoT concepts and LoRa communication

See the `examples/RaspberryPi_Pico/` folder for complete MicroPython examples.


## RYLR998 Applications

The RYLR998's long-range, low-power characteristics make it ideal for various IoT applications:

### Smart Agriculture
- Soil moisture monitoring across large farms
- Weather station data collection
- Irrigation control systems
- Livestock tracking

### Smart Cities
- Parking space monitoring
- Street lighting control
- Waste management (bin level monitoring)
- Environmental monitoring (air quality, noise)

### Industrial IoT
- Remote sensor monitoring
- Asset tracking in warehouses
- Machine-to-machine communication
- Alarm and security systems

### Home Automation
- Long-range sensor networks
- Gate and door controls
- Environmental monitoring
- Energy consumption tracking

### Emergency & Safety
- Disaster warning systems
- Remote alarm systems
- Search and rescue communication
- Off-grid communication networks




## References

* [RYLR998 - 868/915MHz LoRa® Antenna Transceiver Module](https://reyax.com/products/rylr998/)
* [LoRa Distance Testing with RYLR998 in the open field - Amazing results!](https://www.tastethecode.com/lora-distance-testing-with-rylr998)
* [LoRa Distance Testing With RYLR998 in Open Field - Amazing Results!](https://www.instructables.com/LoRa-Distance-Testing-With-RYLR998-in-Open-Field-A/)
* [Greenhouse Monitoring System - LoRa IoT](https://hackaday.io/project/191787-greenhouse-monitoring-system-lora-iot)
* [Reyax-LoRa-Module-RYLR998-Review](https://github.com/make2explore/Reyax-LoRa-Module-RYLR998-Review)
