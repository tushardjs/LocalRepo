
#include<Arduino.h>
#include <Adafruit_INA219.h>
#include <Wire.h>
#include<config.h>
Adafruit_INA219 ina219;


ulong lastSensorReadTime = 0;
//sends current sensor data to serial monitor

void readCurrentSensorData()
{ 
    if (millis() - lastSensorReadTime >= sensorReadInterval)
  {
    lastSensorReadTime = millis();

    float shuntvoltage = ina219.getShuntVoltage_mV();
    float busvoltage = ina219.getBusVoltage_V();
    float current_mA = ina219.getCurrent_mA();
    float power_mW = ina219.getPower_mW();
    float calculatedPower_mW = busvoltage * current_mA;

    Serial.print("Bus Voltage:   ");
    Serial.print(busvoltage);
    Serial.println(" V");
    Serial.print("Shunt Voltage: ");
    Serial.print(shuntvoltage);
    Serial.println(" mV");
    Serial.print("Load Voltage:  ");
    Serial.print(busvoltage + (shuntvoltage / 1000));
    Serial.println(" V");
    Serial.print("Current:       ");
    Serial.print(current_mA);
    Serial.println(" mA");
    Serial.print("Power (INA219):");
    Serial.print(power_mW);
    Serial.println(" mW");
    Serial.print("Power (Calculated): ");
    Serial.print(calculatedPower_mW);
    Serial.println(" mW");
    Serial.println("");
  }
}