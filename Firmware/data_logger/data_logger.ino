#include <SPI.h>
#include <SD.h>
#include "sd_wrapper.h"
#include "timestamp_handler.h"

// SD card config
const int CHIP_SELECT = 10;  // Default for Arduino Ethernet shield
const int LOG_INTERVAL = 10; // ms (matches visualization rate)

// Data buffer
struct SensorData {
  unsigned long timestamp;
  float z1;
  float z2;
} dataBuffer[100];  // Buffer for 1s at 100Hz

int bufferIndex = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Wait for serial monitor
  
  if (!initSD(CHIP_SELECT)) {
    Serial.println("SD init failed!");
    while(1);  // Halt on failure
  }
  
  createNewFile();  // Generates timestamped filename
}

void loop() {
  static unsigned long lastLog = 0;
  
  if (millis() - lastLog >= LOG_INTERVAL) {
    lastLog = millis();
    
    // Simulate sensor read (replace with actual accelerometer code)
    dataBuffer[bufferIndex] = {
      .timestamp = lastLog,
      .z1 = analogRead(A0) * 0.1,
      .z2 = analogRead(A1) * 0.1
    };
    
    if (++bufferIndex >= 100) {
      writeBufferToSD(dataBuffer, 100);
      bufferIndex = 0;
    }
  }
}
