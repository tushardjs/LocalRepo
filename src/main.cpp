#include <Arduino.h>
#include <readCurrentSensor.h>
#include <EspNowRecive.h>
#include "soc/rtc_cntl_reg.h"
#include "soc/soc.h"

// Global variables

volatile unsigned long lastDebounceTimeOpen = 0;  // Last debounce time for open switch
volatile unsigned long lastDebounceTimeClose = 0; // Last debounce time for close switch
volatile bool openInterruptTriggered = false;     // Flag to indicate if the open limit switch was triggered
volatile bool closeInterruptTriggered = false;    // Flag to indicate if the close limit switch was triggered

// Setup function to initialize hardware and configurations
void setup()
{
  // Disable brownout detection to prevent unexpected resets
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  // Initialize serial communication for debugging
  Serial.begin(115200);
  delay(1000); // Wait for serial monitor to initialize

  // Set WiFi mode to Station (required for ESP-NOW)
  WiFi.mode(WIFI_STA);
  Serial.println("ESPNow Example");
  Serial.println("This is the receiver");

  // Print the MAC address of the ESP32
  Serial.print("My MAC Address is: ");
  Serial.println(WiFi.macAddress());

  // Initialize the INA219 current sensor
  if (!ina219.begin())
  {
    Serial.println("Failed to find INA219 chip");
  }
  Serial.println("Measuring voltage and current with INA219 ...");

  // Initialize ESP-NOW for communication
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ESPNow Init Failed");
    delay(3000);   // Wait before restarting
    ESP.restart(); // Restart the ESP32 if initialization fails
  }

  // Initialize motor driver
  motorDriver.begin();

  // Register callback function for receiving ESP-NOW messages
  esp_now_register_recv_cb(receiveCallback);

  // Set up interrupt service routines (ISRs) for limit switches
  attachInterrupt(digitalPinToInterrupt(openLimitSwitchPin), openLimitSwitchInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(closeLimitSwitchPin), closeLimitSwitchInterrupt, FALLING);

  Serial.println("Pins initialized.");
}

// Main loop function
void loop()
{
  // Print the status of interrupts if triggered
  printInterruptTriggered();
  motorDriver.loop();
  // Read current sensor data
  // readCurrentSensorData();
}

// Interrupt service routine for the open limit switch
void openLimitSwitchInterrupt()
{

  // Check if debounce delay has passed
  if ((millis() - lastDebounceTimeOpen) > debounceDelay)
  {
    lastDebounceTimeOpen = millis();         // Update the last debounce time
   // if (digitalRead(openLimitSwitchPin) == LOW) // LOW means pressed
   // {
      motorDriver.stop();            // Stop the motor
      openInterruptTriggered = true; // Set the flag to indicate the interrupt
  //  }
  }
}

// Interrupt service routine for the close limit switch
void closeLimitSwitchInterrupt()
  {
    // Check if debounce delay has passed
    if ((currentTime - lastDebounceTimeClose) > debounceDelay)
    {
    
      lastDebounceTimeClose = currentTime; // Update the last debounce time
        if (digitalRead(closeLimitSwitchPin) == LOW) 
        {
      motorDriver.stop();             // Stop the motor
      closeInterruptTriggered = true; // Set the flag to indicate the interrupt
    }
    }
  }
// Function to print the status of limit switch interrupts
void printInterruptTriggered()
{
  // Check if the open limit switch interrupt was triggered
  if (openInterruptTriggered)
  {
    Serial.println("Open Limit Switch Interrupt"); // Print status message
    openInterruptTriggered = false;                // Reset the flag
  }

  // Check if the close limit switch interrupt was triggered
  if (closeInterruptTriggered)
  {
    Serial.println("Close Limit Switch Interrupt"); // Print status message
    closeInterruptTriggered = false;                // Reset the flag
  }
}
