#include <Arduino.h>

// Define the pin numbers for various components

// Pin connected to the LED
uint8_t ledPin = 2;

// Pins connected to the motor driver
uint8_t motorPin_1 = 14;
uint8_t motorPin_2 = 12;

// Pins connected to the limit switches
uint8_t openLimitSwitchPin = 16;
uint8_t closeLimitSwitchPin = 17;

  // Debounce delay for interrupt handling
uint8_t debounceDelay = 200;          

// Interval for reading current sensor data
int sensorReadInterval = 2000;

unsigned long currentTime = millis();

// Function prototypes

// Callback function to handle incoming messages via ESP-NOW
void receiveCallback();

// Interrupt service routine (ISR) for the open limit switch
void openLimitSwitchInterrupt();

// Interrupt service routine (ISR) for the close limit switch
void closeLimitSwitchInterrupt();

// Function to print information about triggered interrupts
void printInterruptTriggered();

// Function to format a MAC address into a human-readable string
void formatMacAddress();

// void isrhandel();

// void handleLimitSwitches();