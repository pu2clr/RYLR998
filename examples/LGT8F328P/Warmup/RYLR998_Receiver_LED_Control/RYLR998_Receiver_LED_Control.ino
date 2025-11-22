/*
 * RYLR998 Receiver - LED Control Example
 * 
 * This sketch demonstrates how to receive LoRa commands using the RYLR998 module
 * connected to an LGT8F328P (3.3V Arduino Nano compatible) and control an LED
 * based on received commands.
 * 
 * Hardware Connections:
 * ---------------------
 * RYLR998 TX  -> LGT8F328P Pin 4 (Software Serial RX)
 * RYLR998 RX  -> LGT8F328P Pin 5 (Software Serial TX)
 * RYLR998 VDD -> 3.3V
 * RYLR998 GND -> GND
 * 
 * LED (+)     -> LGT8F328P Pin 6 (with 220Ω resistor)
 * LED (-)     -> GND
 * 
 * Commands:
 * ---------
 * "TURN ON"  - Turns the LED ON
 * "TURN OFF" - Turns the LED OFF
 * 
 * Message Format from RYLR998:
 * +RCV=<address>,<length>,<data>,<RSSI>,<SNR>
 * Example: +RCV=100,7,TURN ON,-45,10
 * 
 * Author: PU2CLR
 * Date: November 2025
 * License: MIT
 */

#include <SoftwareSerial.h>

// Pin definitions
#define LORA_RX_PIN 4      // Connect to RYLR998 TX
#define LORA_TX_PIN 5      // Connect to RYLR998 RX
#define LED_PIN 6          // LED control pin

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
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  Serial.println("=================================");
  Serial.println("RYLR998 Receiver - LED Control");
  Serial.println("=================================");
  Serial.println();
  
  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Start with LED OFF
  Serial.println("LED initialized (Pin 6) - OFF");
  
  // Initialize LoRa serial communication
  loraSerial.begin(LORA_BAUD_RATE);
  delay(100);
  
  // Configure RYLR998 module
  Serial.println("Configuring RYLR998 module...");
  configureLoRaModule();
  
  Serial.println();
  Serial.println("System ready! Waiting for commands...");
  Serial.println("Valid commands: 'TURN ON' or 'TURN OFF'");
  Serial.println("=================================");
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
