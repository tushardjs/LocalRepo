#include<Arduino.h>
#include <esp_now.h>
#include <ArduinoJson.h>
// Receives message from the transmitter
void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen)
{
  char buffer[ESP_NOW_MAX_DATA_LEN + 1];
  int msgLen = min(ESP_NOW_MAX_DATA_LEN, dataLen);
  strncpy(buffer, (const char *)data, msgLen);
  buffer[msgLen] = 0;

  char macStr[18];
  formatMacAddress(macAddr, macStr, 18);
  Serial.printf("Received message from: %s - %s\n", macStr, buffer);

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, buffer);

  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  const char *command = doc["command"];
  if (command)
  {
    if (strcmp(command, "on") == 0)
    {
      motorDriver.valveClockWise();
    }
    else if (strcmp(command, "stop") == 0)
    {
      motorDriver.stop();
    }
    else if (strcmp(command, "off") == 0)
    {
      motorDriver.valveCounterClockWise();
    }
  }
}