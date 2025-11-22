/*
 * RYLR998 Transmitter - LED Control Example
 * 
 * This sketch demonstrates how to send LoRa commands using the RYLR998 module
 * connected to an LGT8F328P (3.3V Arduino Nano compatible) to control a remote LED.
 * 
 * Hardware Connections:
 * ---------------------
 * RYLR998 TX  -> LGT8F328P Pin 4 (Software Serial RX)
 * RYLR998 RX  -> LGT8F328P Pin 5 (Software Serial TX)
 * RYLR998 VDD -> 3.3V
 * RYLR998 GND -> GND
 * 
 * Button 1    -> LGT8F328P Pin 7 (with pull-up resistor) - TURN ON
 * Button 2    -> LGT8F328P Pin 8 (with pull-up resistor) - TURN OFF
 * 
 * Commands Sent:
 * --------------
 * Button 1 pressed -> Sends "TURN ON" command
 * Button 2 pressed -> Sends "TURN OFF" command
 * 
 * You can also send commands via Serial Monitor:
 * Type "ON" or "OFF" in the Serial Monitor
 * 
 * Author: PU2CLR
 * Date: November 2025
 * License: MIT
 */

#include <SoftwareSerial.h>

// Pin definitions
#define LORA_RX_PIN 4      // Connect to RYLR998 TX
#define LORA_TX_PIN 5      // Connect to RYLR998 RX
#define BUTTON_ON_PIN 7    // Button to send TURN ON
#define BUTTON_OFF_PIN 8   // Button to send TURN OFF

// RYLR998 module configuration
#define LORA_BAUD_RATE 115200
#define DEVICE_ADDRESS 6    // This device address
#define TARGET_ADDRESS 1    // Receiver device address
#define NETWORK_ID 6          // Network ID (must match receiver)

// Create SoftwareSerial object for RYLR998
SoftwareSerial loraSerial(LORA_RX_PIN, LORA_TX_PIN);

// Button debounce variables
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 300;  // 300ms debounce

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  Serial.println("====================================");
  Serial.println("RYLR998 Transmitter - LED Control");
  Serial.println("====================================");
  Serial.println();
  
  // Initialize button pins with internal pull-up resistors
  pinMode(BUTTON_ON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_OFF_PIN, INPUT_PULLUP);
  Serial.println("Buttons initialized:");
  Serial.println("  Pin 7: TURN ON button");
  Serial.println("  Pin 8: TURN OFF button");
  
  // Initialize LoRa serial communication
  loraSerial.begin(LORA_BAUD_RATE);
  delay(100);
  
  // Configure RYLR998 module
  Serial.println();
  Serial.println("Configuring RYLR998 module...");
  configureLoRaModule();
  
  Serial.println();
  Serial.println("System ready!");
  Serial.println("====================================");
  Serial.println("Press buttons or type commands:");
  Serial.println("  Button Pin 7 or type 'ON'  -> Send TURN ON");
  Serial.println("  Button Pin 8 or type 'OFF' -> Send TURN OFF");
  Serial.println("====================================");
  Serial.println();
}

void loop() {
  // Check button states
  checkButtons();
  
  // Check for Serial commands
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toUpperCase();
    
    if (command == "ON") {
      sendLoRaCommand("TURN ON");
    } 
    else if (command == "OFF") {
      sendLoRaCommand("TURN OFF");
    }
    else if (command.startsWith("AT")) {
      // Allow direct AT commands for testing
      sendATCommand(command);
    }
    else {
      Serial.println("Unknown command. Use 'ON' or 'OFF'");
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
 * Check button states and send commands
 */
void checkButtons() {
  unsigned long currentTime = millis();
  
  // Check TURN ON button (active LOW because of pull-up)
  if (digitalRead(BUTTON_ON_PIN) == LOW) {
    if (currentTime - lastButtonPress > debounceDelay) {
      Serial.println();
      Serial.println(">>> Button ON pressed <<<");
      sendLoRaCommand("TURN ON");
      lastButtonPress = currentTime;
    }
  }
  
  // Check TURN OFF button (active LOW because of pull-up)
  if (digitalRead(BUTTON_OFF_PIN) == LOW) {
    if (currentTime - lastButtonPress > debounceDelay) {
      Serial.println();
      Serial.println(">>> Button OFF pressed <<<");
      sendLoRaCommand("TURN OFF");
      lastButtonPress = currentTime;
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
