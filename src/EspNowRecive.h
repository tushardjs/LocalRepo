#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <ArduinoJson.h>
#include <motorDriver.h>

// Instantiate the motor driver with pins for motor control and limit switches
MotorDriver motorDriver(motorPin_1, motorPin_2, openLimitSwitchPin, closeLimitSwitchPin);

// Convert a MAC address to a human-readable string format
void formatMacAddress(const uint8_t *macAddr, char *buffer, int maxLength)
{
  snprintf(buffer, maxLength, "%02x:%02x:%02x:%02x:%02x:%02x", 
           macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
}

// Callback function to handle incoming messages via ESP-NOW
void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen)
{
  // Buffer to hold the received data as a string
  char buffer[ESP_NOW_MAX_DATA_LEN + 1];
  int msgLen = min(ESP_NOW_MAX_DATA_LEN, dataLen);  // Ensure message length does not exceed buffer size
  strncpy(buffer, (const char *)data, msgLen);  // Copy the data into buffer
  buffer[msgLen] = 0;  // Null-terminate the string

  // Convert MAC address to a human-readable string format
  char macStr[18];
  formatMacAddress(macAddr, macStr, 18);

  // Print received message and sender's MAC address
  Serial.printf("Received message from: %s - %s\n", macStr, buffer);

  // Create a JSON document to parse the received data
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, buffer);  // Parse the JSON data

  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());  
    return;
  }

  // Extract the command from the JSON document
  const char *command = doc["command"];
  if (command)
  {
    // Execute actions based on the received command
    if (strcmp(command, "on") == 0)
    {
      motorDriver.valveClockWise();  // Rotate the valve clockwise
    }
    else if (strcmp(command, "stop") == 0)
    {
      motorDriver.stop();  // Stop the motor
    }
    else if (strcmp(command, "off") == 0)
    {
      motorDriver.valveCounterClockWise();  // Rotate the valve counterclockwise
    }
  }
}
