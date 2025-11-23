/*
 * RYLR998 Transmitter - Range Testing Tool
 * 
 * This sketch automatically sends alternating LoRa commands (TURN ON/TURN OFF) every
 * 1 second to test the range and coverage of your LoRa network. The remote LED will
 * blink continuously, allowing you to verify signal reception at different locations.
 * 
 * Use Case:
 * --------
 * - Place this transmitter at a strategic location (fixed position)
 * - Walk around with the receiver to test coverage area
 * - Monitor LED blinking and Serial output to verify connectivity
 * - Check RSSI and SNR values to assess signal quality
 * - Identify dead zones or areas with weak signal
 * - Optimize antenna placement and orientation
 * 
 * Hardware Connections:
 * ---------------------
 * RYLR998 TX  -> LGT8F328P Pin 4 (Software Serial RX)
 * RYLR998 RX  -> LGT8F328P Pin 5 (Software Serial TX)
 * RYLR998 VDD -> 3.3V
 * RYLR998 GND -> GND
 * 
 * No buttons required - automatic operation!
 * 
 * Operation:
 * ----------
 * - Automatically sends "TURN ON" command
 * - Waits 1 second (1000ms)
 * - Automatically sends "TURN OFF" command
 * - Waits 1 second (1000ms)
 * - Repeats indefinitely
 * 
 * The receiver LED will blink at 1-second intervals if within range.
 * 
 * Author: PU2CLR
 * Date: November 2025
 * License: MIT
 */

#include <SoftwareSerial.h>

// Pin definitions
#define LORA_RX_PIN 4      // Connect to RYLR998 TX
#define LORA_TX_PIN 5      // Connect to RYLR998 RX

// RYLR998 module configuration
#define LORA_BAUD_RATE 115200
#define DEVICE_ADDRESS 6      // This device address
#define TARGET_ADDRESS 1      // Receiver device address
#define NETWORK_ID 6          // Network ID (must match receiver)

// Range testing configuration
#define BLINK_INTERVAL 1000   // 1000ms = 1 second between commands

// Create SoftwareSerial object for RYLR998
SoftwareSerial loraSerial(LORA_RX_PIN, LORA_TX_PIN);

// Timing and state variables
unsigned long lastTransmission = 0;
bool ledState = false;  // false = OFF, true = ON
unsigned long messageCount = 0;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  Serial.println("==========================================");
  Serial.println("RYLR998 Range Testing Transmitter");
  Serial.println("==========================================");
  Serial.println();
  Serial.println("This device will automatically send");
  Serial.println("TURN ON/OFF commands every 1 second.");
  Serial.println();
  Serial.println("Use this to test LoRa coverage:");
  Serial.println("1. Place this transmitter at a fixed location");
  Serial.println("2. Walk around with the receiver");
  Serial.println("3. Monitor LED blinking and RSSI/SNR values");
  Serial.println("4. Identify coverage areas and dead zones");
  Serial.println();
  
  // Initialize LoRa serial communication
  loraSerial.begin(LORA_BAUD_RATE);
  delay(100);
  
  // Configure RYLR998 module
  Serial.println();
  Serial.println("Configuring RYLR998 module...");
  configureLoRaModule();
  
  Serial.println();
  Serial.println("System ready!");
  Serial.println("==========================================");
  Serial.println("Starting automatic transmission...");
  Serial.println("Sending commands every 1 second");
  Serial.println("Monitor receiver for LED blinking");
  Serial.println("==========================================");
  Serial.println();
  
  // Initialize timing
  lastTransmission = millis();
}

void loop() {
  unsigned long currentTime = millis();
  
  // Check if it's time to send next command
  if (currentTime - lastTransmission >= BLINK_INTERVAL) {
    lastTransmission = currentTime;
    messageCount++;
    
    // Toggle LED state and send appropriate command
    if (ledState) {
      sendLoRaCommand("TURN OFF");
      ledState = false;
    } else {
      sendLoRaCommand("TURN ON");
      ledState = true;
    }
    
    // Display statistics
    Serial.print("[Message #");
    Serial.print(messageCount);
    Serial.print("] Next transmission in ");
    Serial.print(BLINK_INTERVAL / 1000);
    Serial.println(" second(s)");
    Serial.println();
  }
  
  // Check for manual AT commands via Serial Monitor (for testing)
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.startsWith("AT")) {
      Serial.println("[Manual AT Command]");
      sendATCommand(command);
    } else {
      Serial.println("Only AT commands accepted in auto mode.");
      Serial.println("Example: AT+PARAMETER?");
    }
  }
  
  // Check for responses from RYLR998
  if (loraSerial.available()) {
    String response = loraSerial.readStringUntil('\n');
    response.trim();
    if (response.length() > 0) {
      Serial.print("LoRa: ");
      Serial.println(response);
    }
  }
}

/**
 * Configure the RYLR998 module
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
 * Send AT command to RYLR998
 */
void sendATCommand(String command) {
  loraSerial.println(command);
  Serial.print("Sent: ");
  Serial.println(command);
  
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
 * Send LoRa command to remote device
 * Format: AT+SEND=<address>,<length>,<data>
 */
void sendLoRaCommand(String command) {
  int dataLength = command.length();
  String atCommand = "AT+SEND=" + String(TARGET_ADDRESS) + "," + 
                     String(dataLength) + "," + command;
  
  Serial.println("--- Sending Command ---");
  Serial.print("Target Address: ");
  Serial.println(TARGET_ADDRESS);
  Serial.print("Command: ");
  Serial.println(command);
  Serial.print("Data Length: ");
  Serial.println(dataLength);
  
  loraSerial.println(atCommand);
  Serial.print("AT Command: ");
  Serial.println(atCommand);
  
  // Wait for response
  delay(200);
  bool responseReceived = false;
  unsigned long startTime = millis();
  
  while (millis() - startTime < 1000) {  // Wait up to 1 second
    if (loraSerial.available()) {
      String response = loraSerial.readStringUntil('\n');
      response.trim();
      if (response.length() > 0) {
        Serial.print("Response: ");
        Serial.println(response);
        responseReceived = true;
        
        if (response == "+OK") {
          Serial.println("✓ Command sent successfully!");
        } else if (response.startsWith("+ERR")) {
          Serial.println("✗ Error sending command!");
        }
      }
    }
  }
  
  if (!responseReceived) {
    Serial.println("⚠ No response from module");
  }
  
  Serial.println("-----------------------");
  Serial.println();
}
