/*
 * RYLR998 Receiver - Proof of Concept with Signal Quality Indicator
 * 
 * This sketch serves as a simple Proof of Concept (PoC) tool for testing LoRa RYLR998
 * communication. It receives LoRa commands, controls an LED, and provides visual feedback
 * of signal quality through a traffic light indicator (green/yellow/red LEDs).
 * 
 * Proof of Concept Features:
 * --------------------------
 * 1. Remote LED Control: Validates basic LoRa command transmission
 * 2. Signal Quality Monitoring: Visual RSSI-based quality indicator
 * 3. Range Testing: Combined with transmitter for coverage assessment
 * 4. Serial Debugging: Detailed message analysis with RSSI/SNR values
 * 
 * Use Cases:
 * ----------
 * - Verify LoRa module connectivity and configuration
 * - Test signal coverage in different locations
 * - Assess signal quality before deploying final application
 * - Learn LoRa basics with immediate visual feedback
 * - Validate network parameters and antenna setup
 * 
 * Hardware Connections:
 * ---------------------
 * RYLR998 TX  -> LGT8F328P Pin 4 (Software Serial RX)
 * RYLR998 RX  -> LGT8F328P Pin 5 (Software Serial TX)
 * RYLR998 VDD -> 3.3V
 * RYLR998 GND -> GND
 * 
 * Command LED (Blinks with commands):
 * LED (+)     -> LGT8F328P Pin 6 (with 220Ω resistor) -> GND
 * 
 * Signal Quality Indicator (Traffic Light):
 * Green LED   -> LGT8F328P Pin 7 (with 220Ω resistor) -> GND  [Excellent signal]
 * Yellow LED  -> LGT8F328P Pin 8 (with 220Ω resistor) -> GND  [Good signal]
 * Red LED     -> LGT8F328P Pin 9 (with 220Ω resistor) -> GND  [Weak signal]
 * 
 * Signal Quality Thresholds:
 * --------------------------
 * GREEN  (Excellent): RSSI > -80 dBm  - Strong signal, optimal for communication
 * YELLOW (Good):      RSSI -80 to -100 dBm - Acceptable signal, reliable communication
 * RED    (Weak):      RSSI < -100 dBm - Weak signal, may have packet loss
 * 
 * Commands:
 * ---------
 * "TURN ON"  - Turns the command LED ON
 * "TURN OFF" - Turns the command LED OFF
 * 
 * Message Format from RYLR998:
 * +RCV=<address>,<length>,<data>,<RSSI>,<SNR>
 * Example: +RCV=6,7,TURN ON,-78,12
 * 
 * Author: PU2CLR
 * Date: November 2025
 * License: MIT
 */

#include <SoftwareSerial.h>

// Pin definitions
#define LORA_RX_PIN 4      // Connect to RYLR998 TX
#define LORA_TX_PIN 5      // Connect to RYLR998 RX
#define LED_PIN 6          // Command LED (blinks with commands)

// Signal quality indicator LEDs (Traffic Light)
#define LED_GREEN_PIN 7    // Excellent signal
#define LED_YELLOW_PIN 8   // Good signal
#define LED_RED_PIN 9      // Weak signal

// ============================================================================
// RSSI THRESHOLD CONFIGURATION
// ============================================================================
// Choose ONE of the three profiles below by uncommenting the desired option.
// These thresholds are based on LoRa technical specifications and practical
// field testing experience.
//
// TECHNICAL BACKGROUND:
// ---------------------
// - RYLR998 Sensitivity: -148 dBm (manufacturer spec)
// - General LoRa ranges (IEEE/Semtech):
//   * RSSI > -70 dBm:     Excellent (very close, line of sight)
//   * -70 to -90 dBm:     Very Good (reliable communication)
//   * -90 to -110 dBm:    Good (acceptable, some obstacles)
//   * -110 to -125 dBm:   Weak (limit, possible packet loss)
//   * < -125 dBm:         Critical (unstable communication)
//
// HOW TO CHOOSE:
// --------------
// - CONSERVATIVE: For critical applications, no packet loss tolerance
// - BALANCED: Recommended for most applications, good margin of safety
// - AGGRESSIVE: To maximize LoRa range, accepts occasional packet loss
//
// VALIDATION METHOD:
// ------------------
// 1. During field tests, note RSSI values at different distances
// 2. Count packet losses at each location (transmitter sends every 1 second)
// 3. Adjust thresholds based on your requirements:
//    - Green:  0% packet loss acceptable
//    - Yellow: 0-5% packet loss acceptable
//    - Red:    >5% packet loss (edge of coverage)
// ============================================================================

// Option 1: CONSERVATIVE Profile (High reliability, safety margin)
// Use for: Critical applications, environments with variable interference
// #define RSSI_EXCELLENT -75   // Green LED threshold
// #define RSSI_GOOD -95        // Yellow/Red LED boundary

// Option 2: BALANCED Profile (Recommended for most applications)
// Use for: General IoT applications, good balance between range and reliability
#define RSSI_EXCELLENT -85   // Green LED threshold
#define RSSI_GOOD -110       // Yellow/Red LED boundary

// Option 3: AGGRESSIVE Profile (Maximum range)
// Use for: Non-critical monitoring, maximizing coverage area
// #define RSSI_EXCELLENT -90   // Green LED threshold
// #define RSSI_GOOD -115       // Yellow/Red LED boundary

// ============================================================================
// SIGNAL QUALITY INTERPRETATION (based on selected profile)
// ============================================================================
// GREEN LED:  RSSI > RSSI_EXCELLENT
//   - Strong signal, optimal for deployment
//   - Expect 0% packet loss
//   - Suitable for real-time critical applications
//
// YELLOW LED: RSSI between RSSI_EXCELLENT and RSSI_GOOD
//   - Acceptable signal for LoRa technology
//   - Reliable communication expected
//   - Suitable for most IoT applications
//
// RED LED:    RSSI < RSSI_GOOD
//   - Weak signal, approaching coverage limit
//   - May experience occasional packet loss
//   - Consider optimization (antenna, position, repeaters)
// ============================================================================

// RYLR998 module configuration
#define LORA_BAUD_RATE 115200
#define DEVICE_ADDRESS 5    // This device address
#define NETWORK_ID 1        // Network ID (must match transmitter)

// Create SoftwareSerial object for RYLR998
SoftwareSerial loraSerial(LORA_RX_PIN, LORA_TX_PIN);

// Variables
String receivedData = "";
bool ledState = false;

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("=================================================");
  Serial.println("RYLR998 Receiver - Proof of Concept Tool");
  Serial.println("With Signal Quality Indicator");
  Serial.println("=================================================");
  Serial.println();
  
  // Initialize command LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.println("Command LED initialized (Pin 6)");
  
  // Initialize signal quality indicator LEDs
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  
  // Initial LED test - traffic light sequence
  Serial.println("Testing signal quality LEDs...");

  digitalWrite(LED_PIN, HIGH);
  digitalWrite(LED_GREEN_PIN, HIGH);
  delay(500);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, HIGH);
  delay(500);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_RED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  

  Serial.println("Signal Quality Indicator ready:");
  Serial.println("  Pin 7 (GREEN)  = Excellent signal (RSSI > -80 dBm)");
  Serial.println("  Pin 8 (YELLOW) = Good signal (-80 to -100 dBm)");
  Serial.println("  Pin 9 (RED)    = Weak signal (< -100 dBm)");
  Serial.println();
  
  // Initialize LoRa serial communication
  loraSerial.begin(LORA_BAUD_RATE);
  delay(100);
  
  // Configure RYLR998 module
  Serial.println("Configuring RYLR998 module...");
  configureLoRaModule();
  
  Serial.println();
  Serial.println("=================================================");
  Serial.println("System ready! Waiting for LoRa messages...");
  Serial.println("Monitor both command LED and signal quality LEDs");
  Serial.println("=================================================");
  Serial.println();
}

void loop() {
  // Check if data is available from RYLR998
  if (loraSerial.available()) {
    char c = loraSerial.read();
    
    // Build the received string
    if (c == '\n') {
      // Process complete message
      processReceivedMessage(receivedData);
      receivedData = "";  // Clear buffer
    } else if (c != '\r') {
      receivedData += c;
    }
  }
  
  // Also monitor Serial for manual AT commands (for testing)
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    sendATCommand(command);
  }
}

/**
 * Configure the RYLR998 module with address and network ID
 */
void configureLoRaModule() {
  // Test communication
  Serial.print("Testing communication... ");
  sendATCommand("AT");
  delay(500);
  
  // Set device address
  Serial.print("Setting address to ");
  Serial.print(DEVICE_ADDRESS);
  Serial.print("... ");
  sendATCommand("AT+ADDRESS=" + String(DEVICE_ADDRESS));
  delay(500);
  
  // Set network ID
  Serial.print("Setting network ID to ");
  Serial.print(NETWORK_ID);
  Serial.print("... ");
  sendATCommand("AT+NETWORKID=" + String(NETWORK_ID));
  delay(500);
  
  // Get current parameters
  Serial.println("Getting module parameters...");
  sendATCommand("AT+PARAMETER?");
  delay(500);
  
  Serial.println("Configuration complete!");
}

/**
 * Send AT command to RYLR998 module
 */
void sendATCommand(String command) {
  loraSerial.println(command);
  Serial.print("Sent: ");
  Serial.println(command);
  
  // Wait for response
  delay(100);
  while (loraSerial.available()) {
    String response = loraSerial.readStringUntil('\n');
    response.trim();
    if (response.length() > 0) {
      Serial.print("Response: ");
      Serial.println(response);
    }
  }
}

/**
 * Display signal quality using traffic light LEDs
 * Based on RSSI value
 */
void displaySignalQuality(int rssi) {
  // Turn off all signal LEDs first
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  
  // Display quality based on RSSI thresholds
  if (rssi > RSSI_EXCELLENT) {
    // Excellent signal - Green LED
    digitalWrite(LED_GREEN_PIN, HIGH);
    Serial.println("Signal Quality: EXCELLENT (Green)");
  } 
  else if (rssi > RSSI_GOOD) {
    // Good signal - Yellow LED
    digitalWrite(LED_YELLOW_PIN, HIGH);
    Serial.println("Signal Quality: GOOD (Yellow)");
  } 
  else {
    // Weak signal - Red LED
    digitalWrite(LED_RED_PIN, HIGH);
    Serial.println("Signal Quality: WEAK (Red)");
  }
}

/**
 * Process received LoRa message
 * Format: +RCV=<address>,<length>,<data>,<RSSI>,<SNR>
 */
void processReceivedMessage(String message) {
  message.trim();
  
  // Check if it's a received message
  if (message.startsWith("+RCV=")) {
    Serial.println();
    Serial.println("--- Message Received ---");
    Serial.print("Raw: ");
    Serial.println(message);
    
    // Parse the message
    // Format: +RCV=100,7,TURN ON,-45,10
    int firstComma = message.indexOf(',');
    int secondComma = message.indexOf(',', firstComma + 1);
    int thirdComma = message.indexOf(',', secondComma + 1);
    int fourthComma = message.indexOf(',', thirdComma + 1);
    
    if (firstComma > 0 && secondComma > 0 && thirdComma > 0) {
      // Extract components
      String senderAddress = message.substring(5, firstComma);
      String dataLength = message.substring(firstComma + 1, secondComma);
      String data = message.substring(secondComma + 1, thirdComma);
      String rssi = message.substring(thirdComma + 1, fourthComma);
      String snr = message.substring(fourthComma + 1);
      
      // Display message details
      Serial.println("Sender Address: " + senderAddress);
      Serial.println("Data Length: " + dataLength);
      Serial.println("Command: " + data);
      Serial.println("RSSI: " + rssi + " dBm");
      Serial.println("SNR: " + snr + " dB");
      
      // Convert RSSI to integer and display signal quality
      int rssiValue = rssi.toInt();
      displaySignalQuality(rssiValue);
      
      // Process the command
      processCommand(data);
    }
    
    Serial.println("------------------------");
    Serial.println();
    
  } else if (message.length() > 0) {
    // Print other responses (like +OK, +ERR, etc.)
    Serial.print("Module: ");
    Serial.println(message);
  }
}

/**
 * Process LED control commands
 */
void processCommand(String command) {
  command.trim();
  command.toUpperCase();  // Convert to uppercase for case-insensitive comparison
  
  if (command == "TURN ON") {
    digitalWrite(LED_PIN, HIGH);
    ledState = true;
    Serial.println(">>> LED turned ON <<<");
  } 
  else if (command == "TURN OFF") {
    digitalWrite(LED_PIN, LOW);
    ledState = false;
    Serial.println(">>> LED turned OFF <<<");
  } 
  else {
    Serial.print(">>> Unknown command: ");
    Serial.print(command);
    Serial.println(" <<<");
    Serial.println("Valid commands: 'TURN ON' or 'TURN OFF'");
  }
  
  // Display current LED state
  Serial.print("LED State: ");
  Serial.println(ledState ? "ON" : "OFF");
}
