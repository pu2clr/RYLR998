# LGT8F328P Warmup Examples

## About This Section

This **Warmup** folder contains **Proof of Concept (PoC)** examples designed to help you quickly validate and test the RYLR998 LoRa module functionality before committing to a full-scale project. These examples serve as simple, practical tools for:

- **Proof of Concept Validation**: Verify LoRa technology meets your project requirements
- **Range and Coverage Testing**: Assess real-world communication distance and signal quality
- **Technology Evaluation**: Understand LoRa capabilities and limitations hands-on
- **Quick Prototyping**: Get a working system in minutes, not hours
- **Learning Platform**: Build confidence with LoRa technology through immediate feedback

### What Makes This a Good PoC?

**Simple Setup:**
- Minimal hardware requirements (2 microcontrollers, 2 LoRa modules, LEDs)
- No complex sensors or external dependencies
- Quick assembly on breadboard

**Immediate Feedback:**
- Visual indicators (LEDs) show system status at a glance
- No need to constantly monitor Serial output
- Traffic light signal quality indicator for intuitive assessment

**Real-World Testing:**
- Battery-powered transmitter for mobility
- Actual range testing in your deployment environment
- RSSI/SNR metrics for signal quality analysis

**Scalable Foundation:**
- Code architecture easily extends to real applications
- Same principles apply to sensor networks, automation, etc.
- Learning from this PoC transfers directly to production projects

### What You'll Learn

- How to connect RYLR998 to LGT8F328P
- Basic AT command usage
- Sending and receiving LoRa messages
- Parsing received data (address, RSSI, SNR)
- **Range and coverage testing techniques**
- **Signal quality assessment (RSSI/SNR analysis)**
- **Field testing methodology for LoRa networks**
- Simple remote control applications
- Debugging techniques with Serial Monitor
- **Practical deployment considerations**

### Moving Forward

Once you're comfortable with these warmup examples, you'll be ready to explore more advanced topics like:
- Sensor data transmission
- Multi-node networks
- Bidirectional communication
- Low-power sleep modes
- Error handling and retransmission
- Gateway implementations

---

## Using This as a Proof of Concept

### Decision Framework

Use this PoC to answer key questions before committing to LoRa for your project:

**1. Range & Coverage Assessment**
- **Question:** Will LoRa cover my required area?
- **Test:** Walk the entire coverage area with receiver, note signal quality LEDs
- **Success Criteria:** Green or Yellow LEDs in all critical locations
- **Decision:** If mostly Red LEDs, consider additional gateways or different technology

**2. Signal Penetration**
- **Question:** Can LoRa penetrate my building materials?
- **Test:** Place transmitter outside/inside, test receiver in various rooms
- **Success Criteria:** Consistent Green/Yellow in target areas
- **Decision:** Red LEDs indicate need for internal repeaters or alternative placement

**3. Reliability Assessment**
- **Question:** Is the communication reliable enough?
- **Test:** Count missed blinks over 5-10 minutes (should be 300-600 blinks)
- **Success Criteria:** <5% packet loss (< 15-30 missed blinks)
- **Decision:** High packet loss suggests need for optimization or different approach

**4. Power Consumption Validation**
- **Question:** Will battery power last for my application?
- **Test:** Power transmitter with battery, measure current, calculate runtime
- **Success Criteria:** Meets your deployment requirements
- **Decision:** If insufficient, consider sleep modes or different power strategy

### PoC Success Metrics

**Green Light to Proceed:** 
- Green/Yellow signal LEDs in >80% of coverage area
- Packet loss <5% in target locations
- Signal penetrates required obstacles
- Battery life meets requirements (if applicable)

**Yellow - Needs Optimization:** 
- Yellow/Red LEDs in 50-80% of area
- Packet loss 5-15%
- May need antenna improvements or positioning changes

**Red - Reconsider Approach:** 
- Red LEDs or no signal in >50% of area
- Packet loss >15%
- Poor obstacle penetration
- Consider alternative: WiFi, different LoRa setup, or hybrid approach

### Next Steps After PoC

**If PoC is Successful:**
1. Document all signal strength readings with locations
2. Create coverage map showing Green/Yellow/Red zones
3. Plan sensor/actuator placement in Green zones
4. Design production hardware based on learnings
5. Proceed to more complex examples in this repository

**If PoC Needs Optimization:**
1. Try different transmitter locations (higher elevation helps)
2. Test different antenna orientations
3. Adjust LoRa parameters (AT+PARAMETER command)
4. Consider external antennas
5. Re-run PoC after changes

**If PoC Fails:**
1. Analyze why (range, obstacles, interference?)
2. Consider alternative: Sub-GHz mesh networks, 2.4GHz, WiFi, etc.
3. Evaluate hybrid approach (LoRa + WiFi gateway)
4. Document findings for future reference

---

## Available Examples

### 1. RYLR998_Receiver_LED_Control (PoC with Signal Quality Indicator)

**Description:** A comprehensive Proof of Concept receiver that validates LoRa communication and provides real-time signal quality feedback through visual indicators.

**Proof of Concept Features:**

1. **Command Reception Validation**
   - Receives and processes LoRa commands wirelessly
   - Controls LED based on received commands (TURN ON/TURN OFF)
   - Proves bidirectional data transmission capability

2. **Signal Quality Monitoring**
   - Traffic light LED indicator (Green/Yellow/Red)
   - Real-time RSSI-based quality assessment
   - Immediate visual feedback without Serial Monitor

3. **Range Assessment Tool**
   - Walk around with this receiver to test coverage
   - Visual indicators show signal degradation
   - Identifies areas of strong, acceptable, and weak signal

4. **Debugging and Analysis**
   - Detailed Serial output with all message parameters
   - RSSI and SNR values for each received message
   - Message parsing demonstration

**Hardware Requirements:**
- LGT8F328P board
- RYLR998 module
- 4 LEDs (1 white/blue for command, 3 for signal quality)
- 4× 220Ω resistors
- Breadboard and jumper wires
- Optional: Battery pack for mobile testing

**Connections:**
```
RYLR998 Connections:
--------------------
RYLR998 TX  -> LGT8F328P Pin 4 (Software Serial RX)
RYLR998 RX  -> LGT8F328P Pin 5 (Software Serial TX)
RYLR998 VDD -> 3.3V
RYLR998 GND -> GND

LED Indicators:
---------------
Command LED (White/Blue):
  LED (+)         -> Pin 6 -> 220Ω resistor -> GND
  
Signal Quality Traffic Light:
  Green LED (+)   -> Pin 7 -> 220Ω resistor -> GND  [Excellent: RSSI > -80 dBm]
  Yellow LED (+)  -> Pin 8 -> 220Ω resistor -> GND  [Good: -80 to -100 dBm]
  Red LED (+)     -> Pin 9 -> 220Ω resistor -> GND  [Weak: < -100 dBm]
```

**Signal Quality Interpretation:**

The receiver uses configurable RSSI thresholds to indicate signal quality. Three profiles are available in the code:

#### Default Configuration (BALANCED Profile - Recommended)

| LED Color | RSSI Range | Signal Quality | What It Means |
|-----------|------------|----------------|---------------|
| 🟢 **GREEN** | > -85 dBm | **Excellent** | Perfect signal, optimal location for deployment |
| 🟡 **YELLOW** | -85 to -110 dBm | **Good** | Acceptable signal, reliable communication expected |
| 🔴 **RED** | < -110 dBm | **Weak** | Marginal signal, may experience packet loss |

#### Available Threshold Profiles

You can change the profile by editing the `RYLR998_Receiver_LED_Control.ino` file. Simply uncomment your preferred option:

**1. CONSERVATIVE Profile** (High reliability, safety margin)
```cpp
#define RSSI_EXCELLENT -75   // Green threshold
#define RSSI_GOOD -95        // Yellow/Red boundary
```
- **Use for:** Critical applications, no packet loss tolerance
- **Green:** RSSI > -75 dBm (very strong signal only)
- **Yellow:** -75 to -95 dBm (good signal)
- **Red:** < -95 dBm (approaching limit)

**2. BALANCED Profile** (Recommended - Default)
```cpp
#define RSSI_EXCELLENT -85   // Green threshold
#define RSSI_GOOD -110       // Yellow/Red boundary
```
- **Use for:** Most IoT applications, good range/reliability balance
- **Green:** RSSI > -85 dBm (strong signal)
- **Yellow:** -85 to -110 dBm (acceptable for LoRa)
- **Red:** < -110 dBm (weak but may still work)

**3. AGGRESSIVE Profile** (Maximum range)
```cpp
#define RSSI_EXCELLENT -90   // Green threshold
#define RSSI_GOOD -115       // Yellow/Red boundary
```
- **Use for:** Non-critical monitoring, maximizing coverage
- **Green:** RSSI > -90 dBm (decent signal)
- **Yellow:** -90 to -115 dBm (pushing LoRa limits)
- **Red:** < -115 dBm (very weak, unstable)

#### Technical Background

These thresholds are based on:

**RYLR998 Specifications:**
- Receiver sensitivity: -148 dBm (manufacturer specification)
- Typical LoRa operation: -120 to -130 dBm practical limit

**IEEE/Semtech LoRa Guidelines:**
- RSSI > -70 dBm: Excellent (very close, line of sight)
- -70 to -90 dBm: Very Good (reliable communication)
- -90 to -110 dBm: Good (acceptable, some obstacles)
- -110 to -125 dBm: Weak (limit, possible packet loss)
- < -125 dBm: Critical (unstable communication)

**How to Choose Your Profile:**

1. **Run field tests** with the default BALANCED profile
2. **Monitor packet loss** at different locations (transmitter sends every 1 second)
3. **Adjust if needed:**
   - If you see red LEDs but 0% packet loss → use AGGRESSIVE profile
   - If you see yellow LEDs with >5% packet loss → use CONSERVATIVE profile
4. **Document RSSI values** at coverage boundaries for future reference

**Visual Operation:**
```
Transmitter sends "TURN ON" command:
  → Command LED turns ON (Pin 6)
  → Green LED ON (if RSSI is -75 dBm)
  
Transmitter sends "TURN OFF" command:
  → Command LED turns OFF (Pin 6)
  → Yellow LED ON (if RSSI is -92 dBm)
  
At signal boundary:
  → Command LED may blink irregularly
  → Red LED ON (if RSSI is -108 dBm)
  → Indicates you're at edge of coverage
```

**Serial Monitor Output Example:**
```
--- Message Received ---
Raw: +RCV=6,7,TURN ON,-78,12
Sender Address: 6
Data Length: 7
Command: TURN ON
RSSI: -78 dBm
SNR: 12 dB
Signal Quality: EXCELLENT (Green)
>>> LED turned ON <<<
LED State: ON
------------------------
```

**Configuration:**
- Device Address: 5
- Network ID: 1
- Baud Rate: 115200

---

### 2. RYLR998_Transmitter_LED_Control (Range Testing Tool)

**Description:** Automatically sends alternating LoRa commands (TURN ON/TURN OFF) every 1 second to test LoRa network range and coverage. This is a practical tool for assessing signal quality and identifying coverage areas.

**Primary Use Case: Range and Coverage Testing**

This transmitter is specifically designed for field testing your LoRa network:

1. **Setup Phase:**
   - Place the transmitter at a strategic/fixed location (e.g., center of area, building rooftop, etc.)
   - Power it up and verify it's transmitting (check Serial Monitor)

2. **Testing Phase:**
   - Take the receiver and walk around the area you want to cover
   - Monitor the LED blinking (should blink every 1 second if signal is received)
   - Check Serial Monitor for RSSI (signal strength) and SNR (signal quality) values
   - Mark areas where signal is strong, weak, or absent

3. **Analysis:**
   - Strong signal: RSSI > -80 dBm, LED blinks consistently
   - Weak signal: RSSI between -80 and -120 dBm, occasional missed blinks
   - No signal: LED doesn't blink, no messages received
   - Use this data to optimize antenna placement, orientation, or add repeaters

**Hardware Requirements:**
- LGT8F328P board
- RYLR998 module
- Power supply (battery for field testing recommended)
- Breadboard and jumper wires

**Connections:**
```
RYLR998 TX     -> LGT8F328P Pin 4 (Software Serial RX)
RYLR998 RX     -> LGT8F328P Pin 5 (Software Serial TX)
RYLR998 VDD    -> 3.3V
RYLR998 GND    -> GND

No buttons required - fully automatic operation!
```

**Operation:**
- **Automatic Mode:** Sends "TURN ON" and "TURN OFF" commands alternately every 1000ms (1 second)
- **LED Blink Pattern:** Remote LED will blink continuously (1 sec ON, 1 sec OFF) if within range
- **Message Counter:** Displays number of messages sent for tracking
- **Manual AT Commands:** You can still send AT commands via Serial Monitor for configuration

**What You'll See:**
```
Starting automatic transmission...
Sending commands every 1 second

--- Sending Command ---
Target Address: 1
Command: TURN ON
...
[Message #1] Next transmission in 1 second(s)

--- Sending Command ---
Target Address: 1
Command: TURN OFF
...
[Message #2] Next transmission in 1 second(s)
```

**Configuration:**
- Device Address: 6
- Target Address: 1  (receiver address)
- Network ID: 6
- Baud Rate: 115200
- Transmission Interval: 1000ms (1 second)

---

## Getting Started

### Step 1: Hardware Setup

**For Proof of Concept Testing:**

1. **Receiver Setup (Signal Quality Indicator):**
   - Connect RYLR998 to LGT8F328P (Pins 4 & 5)
   - Connect 1 command LED to Pin 6
   - Connect traffic light LEDs:
     - Green LED → Pin 7
     - Yellow LED → Pin 8
     - Red LED → Pin 9
   - All LEDs need 220Ω resistors to GND
   - Power at 3.3V

2. **Transmitter Setup (Range Tester):**
   - Connect RYLR998 to LGT8F328P (Pins 4 & 5)
   - No additional LEDs needed
   - Power at 3.3V
   - **Recommended:** Use battery pack for mobility during testing

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

**For Receiver (PoC with Signal Quality):**
1. Open `RYLR998_Receiver_LED_Control.ino`
2. Upload to the LGT8F328P
3. Open Serial Monitor (115200 baud)
4. **Verify LED test sequence** (Green → Yellow → Red)
5. Wait for "System ready!" message
6. **The device is now ready to receive and display signal quality**

**For Transmitter (Range Testing):**
1. Open `RYLR998_Transmitter_LED_Control.ino`
2. Upload to the LGT8F328P
3. Open Serial Monitor (115200 baud)
4. **System will automatically start transmitting every 1 second**
5. Place the transmitter at your desired test location

### Step 4: Range and Coverage Testing

**Recommended Testing Procedure:**

1. **Preparation:**
   - Upload receiver sketch to one LGT8F328P with LED connected
   - Upload transmitter sketch to another LGT8F328P
   - Power both devices (battery recommended for transmitter mobility)

2. **Baseline Test:**
   - Place both devices close together (1-2 meters)
   - Verify command LED is blinking every 1 second
   - **Note which signal quality LED is ON** (should be GREEN)
   - Check Serial Monitor on receiver for RSSI/SNR values
   - Typical close-range RSSI: -30 to -50 dBm

3. **Initial Threshold Validation:**
   - Start with the **BALANCED profile** (default in code)
   - Record RSSI values at different distances
   - Count missed blinks (packet loss) at each location
   - This data will help you choose the best profile for your environment

3. **Coverage Mapping:**
   - **Fix transmitter position** at strategic location (e.g., center of building, rooftop, etc.)
   - Take receiver and walk to different locations
   - At each test point:
     - **Observe command LED** (should blink every 1 second)
     - **Check signal quality LED** (Green/Yellow/Red)
     - Note RSSI value from Serial Monitor
     - Record SNR (Signal-to-Noise Ratio)
     - Mark location based on traffic light color

4. **Signal Quality Reference (BALANCED Profile - Default):**
   - 🟢 **Excellent (Green LED):** RSSI > -85 dBm, SNR > 10 dB
     - Command LED blinks perfectly every 1 second
     - Ideal location for deployment
     - Expect 0% packet loss
   - 🟡 **Good (Yellow LED):** RSSI -85 to -110 dBm, SNR 5-10 dB
     - Command LED blinks consistently
     - Acceptable for most LoRa applications
     - Expect <5% packet loss
   - 🔴 **Weak (Red LED):** RSSI < -110 dBm, SNR < 5 dB
     - Command LED may miss some blinks
     - At edge of coverage, consider optimization
     - Expect >5% packet loss
   - ⚫ **No Signal:** No LED activity
     - Outside coverage area
     - No communication possible

   > **Note:** These values are based on the BALANCED profile. If you changed to CONSERVATIVE or AGGRESSIVE profile, the thresholds will be different (see code comments).

5. **Validate and Adjust Thresholds (Important!):**
   
   **Testing Method:**
   - At each test location, count packet loss over 5 minutes
   - Transmitter sends 300 messages (1 per second × 5 minutes)
   - Count how many command LED blinks you missed
   - Calculate: Packet Loss % = (Missed / 300) × 100
   
   **Adjustment Guide:**
   - If **Green LED** shows but you have packet loss → Profile is too aggressive
   - If **Red LED** shows but you have 0% packet loss → Profile is too conservative
   - If **Yellow LED** shows with <5% packet loss → Profile is perfect!
   
   **Example from your tests:**
   - 100m with obstacles → Yellow LED, 0% loss → GOOD! ✓
   - 500m with obstacles → Red LED, some loss → Expected for Red ✓
   - This validates the BALANCED profile works well for your environment

6. **Document Results:**
   - Create a coverage map showing LED colors at each location
   - Note RSSI values and packet loss percentages
   - Mark obstacles (walls, metal structures, etc.)
   - Document which profile works best for your environment
   - Test at different times of day if possible
   - Consider antenna height and orientation

7. **Optimization:**
   - If coverage is insufficient:
     - First, try AGGRESSIVE profile to see if communication is still reliable
     - Adjust antenna position/orientation
     - Try different transmitter locations (higher elevation helps)
     - Consider using external antennas
     - Evaluate adding repeaters/gateways
     - Check LoRa parameters (AT+PARAMETER?)

**What to Monitor on Receiver:**

**Visual Indicators (No Serial Monitor Needed!):**
- **Command LED (Pin 6):** Blinks ON/OFF every second when receiving
- **Green LED (Pin 7):** Lights up for excellent signal
- **Yellow LED (Pin 8):** Lights up for good signal  
- **Red LED (Pin 9):** Lights up for weak signal

**Serial Monitor (Detailed Analysis):**
```
--- Message Received ---
Raw: +RCV=6,7,TURN ON,-78,12
Sender Address: 6
Data Length: 7
Command: TURN ON
RSSI: -78 dBm          ← Signal strength
SNR: 12 dB             ← Signal quality
Signal Quality: EXCELLENT (Green)  ← Traffic light status
>>> LED turned ON <<<
LED State: ON
------------------------
```

**Troubleshooting Range Testing:**

**Transmitter not sending:**
- Check Serial Monitor for "Starting automatic transmission..." message
- Verify RYLR998 module is responding to AT commands
- Ensure power supply is stable (especially on battery)

**Inconsistent LED blinking:**
- Normal at signal boundaries (this helps identify coverage limits!)
- Note RSSI/SNR values to understand why
- Try adjusting antenna orientation
- Move to different location to confirm

**No communication between modules:**
- Verify both modules have the same Network ID
- Check wiring connections (TX/RX must be crossed)
- Ensure both devices are powered at 3.3V
- Verify baud rate is 115200 for both modules
- Check device addresses match (transmitter target = receiver address)

**LED not responding:**
- Check LED polarity (long leg = +, short leg = -)
- Verify 220Ω resistor is in series with each LED
- Check pin connections (6, 7, 8, 9)
- Verify receiver is getting commands (check Serial Monitor)

**Signal quality LEDs not working:**
- Verify traffic light test sequence runs at startup
- Check each LED individually with digitalWrite test
- Ensure proper grounding for all LEDs
- Verify RSSI values are being parsed correctly (check Serial output)

**Module not responding to AT commands:**
- Check RX/TX connections (they should be crossed: module TX → MCU RX, module RX → MCU TX)
- Verify baud rate (115200)
- Ensure 3.3V power supply is stable
- Try resetting the module (power cycle)

**Weak signal or short range:**
- Ensure antennas are properly connected
- Try different antenna orientations (vertical usually works best)
- Check for obstacles (metal, concrete walls reduce range significantly)
- Verify LoRa parameters are optimized (use AT+PARAMETER? to check)
- Consider external antennas for better range

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
