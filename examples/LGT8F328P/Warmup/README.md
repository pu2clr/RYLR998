# LGT8F328P Warmup Examples

## About This Section

This **Warmup** folder contains basic introductory examples designed to help you get started with the RYLR998 LoRa module and LGT8F328P microcontroller. These examples use simple, straightforward applications to help you:

- **Understand the basics** of LoRa communication with RYLR998
- **Learn UART communication** between microcontroller and LoRa module
- **Get familiar with AT commands** for module configuration
- **Build confidence** before moving to more complex projects
- **Verify your hardware setup** is working correctly

Think of these as your first steps into the world of LoRa IoT development—simple, practical, and educational!

### What You'll Learn

- How to connect RYLR998 to LGT8F328P
- Basic AT command usage
- Sending and receiving LoRa messages
- Parsing received data (address, RSSI, SNR)
- Simple remote control applications
- Debugging techniques with Serial Monitor

### Moving Forward

Once you're comfortable with these warmup examples, you'll be ready to explore more advanced topics like:
- Sensor data transmission
- Multi-node networks
- Bidirectional communication
- Low-power sleep modes
- Error handling and retransmission
- Gateway implementations

---

## Available Examples

### 1. RYLR998_Receiver_LED_Control

**Description:** Receives LoRa commands and controls an LED based on the received command.

**Hardware Requirements:**
- LGT8F328P board
- RYLR998 module
- LED with 220Ω resistor
- Breadboard and jumper wires

**Connections:**
```
RYLR998 TX  -> LGT8F328P Pin 4 (Software Serial RX)
RYLR998 RX  -> LGT8F328P Pin 5 (Software Serial TX)
RYLR998 VDD -> 3.3V
RYLR998 GND -> GND

LED (+)     -> LGT8F328P Pin 6 (through 220Ω resistor)
LED (-)     -> GND
```

**Commands:**
- `TURN ON` - Turns the LED ON
- `TURN OFF` - Turns the LED OFF

**Configuration:**
- Device Address: 5
- Network ID: 1
- Baud Rate: 115200

---

### 2. RYLR998_Transmitter_LED_Control

**Description:** Sends LoRa commands to control a remote LED. Commands can be sent via buttons or Serial Monitor.

**Hardware Requirements:**
- LGT8F328P board
- RYLR998 module
- 2 push buttons (optional)
- Breadboard and jumper wires

**Connections:**
```
RYLR998 TX     -> LGT8F328P Pin 4 (Software Serial RX)
RYLR998 RX     -> LGT8F328P Pin 5 (Software Serial TX)
RYLR998 VDD    -> 3.3V
RYLR998 GND    -> GND

Button ON      -> LGT8F328P Pin 7 (with internal pull-up)
Button OFF     -> LGT8F328P Pin 8 (with internal pull-up)
Other side     -> GND
```

**Usage:**
- Press button on Pin 7 to send "TURN ON" command
- Press button on Pin 8 to send "TURN OFF" command
- Type `ON` or `OFF` in Serial Monitor to send commands
- Type AT commands directly for module configuration

**Configuration:**
- Device Address: 6
- Target Address: 5
- Network ID: 1
- Baud Rate: 115200

---

## Getting Started

### Step 1: Hardware Setup

1. Connect the RYLR998 module to your LGT8F328P according to the connection diagram
2. Ensure the LGT8F328P is powered at 3.3V
3. For the receiver example, connect an LED to Pin 6
4. For the transmitter example, optionally connect buttons to Pins 7 and 8

### Step 2: Software Setup

1. Install Arduino IDE (1.8.x or 2.x)
2. Install LGT8F328P board support (if not already installed):
   - Open File → Preferences
   - Add to "Additional Boards Manager URLs": 
     ```
     https://raw.githubusercontent.com/dbuezas/lgt8fx/master/package_lgt8fx_index.json
     ```
   - Open Tools → Board → Boards Manager
   - Search for "LGT8F" and install

3. Select the correct board:
   - Tools → Board → LGT8Fx Boards → LGT8F328P
   - Tools → Variant → 328P-LQFP32 MiniEVB
   - Tools → Clock → 16 MHz
   - Select your COM port

### Step 3: Upload and Test

**For Receiver:**
1. Open `RYLR998_Receiver_LED_Control.ino`
2. Upload to the LGT8F328P
3. Open Serial Monitor (115200 baud)
4. Wait for "System ready!" message
5. The device will now listen for LoRa commands

**For Transmitter:**
1. Open `RYLR998_Transmitter_LED_Control.ino`
2. Upload to another LGT8F328P
3. Open Serial Monitor (115200 baud)
4. Press buttons or type commands to control the remote LED

### Step 4: Testing Communication

1. Power up both devices (transmitter and receiver)
2. On the transmitter:
   - Press the button on Pin 7 OR type `ON` in Serial Monitor
   - You should see confirmation in the Serial Monitor
3. On the receiver:
   - Serial Monitor should display received message
   - LED should turn ON
4. To turn OFF:
   - Press button on Pin 8 OR type `OFF` in Serial Monitor
   - LED should turn OFF

## Troubleshooting

**No communication between modules:**
- Verify both modules have the same Network ID (6)
- Check wiring connections
- Ensure both devices are powered at 3.3V
- Verify baud rate is 115200 for both modules

**LED not responding:**
- Check LED polarity
- Verify 220Ω resistor is in series with LED
- Check Pin 6 connection
- Verify receiver is getting the correct command

**Module not responding to AT commands:**
- Check RX/TX connections (they should be crossed)
- Verify baud rate (115200)
- Ensure 3.3V power supply is stable
- Try resetting the module

**Buttons not working:**
- Verify buttons are connected correctly (one side to pin, other to GND)
- Check that internal pull-up resistors are enabled in code
- Test buttons with a multimeter

## Additional Information

### Message Format

**Transmitted:**
```
AT+SEND=<target_address>,<data_length>,<data>
Example: AT+SEND=200,7,TURN ON
```

**Received:**
```
+RCV=<sender_address>,<data_length>,<data>,<RSSI>,<SNR>
Example: +RCV=100,7,TURN ON,-45,10
```

### Customization

You can modify these examples to:
- Change device addresses and network ID
- Add more commands (e.g., "BLINK", "TOGGLE")
- Control multiple LEDs or other devices
- Add sensors and send data
- Implement bidirectional communication
- Add acknowledgment messages

### Pin Change

To use different pins, modify these definitions in the sketch:
```cpp
#define LORA_RX_PIN 4      // Change to your preferred RX pin
#define LORA_TX_PIN 5      // Change to your preferred TX pin
#define LED_PIN 6          // Change to your preferred LED pin
```

## Support

For issues, questions, or contributions, please visit the main repository.

## License

MIT License - See main repository for details.
