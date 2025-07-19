#include <Wire.h>
#include "filters.h"
#include "sensor_calibration.h"

// I2C addresses for dual sensors (AD0 pin determines address)
const uint8_t ACCEL_1_ADDR = 0x68;  // AD0 = GND
const uint8_t ACCEL_2_ADDR = 0x69;  // AD0 = VCC

// Sensor calibration offsets (run calibration script first)
const float SENSOR1_OFFSET_Z = -0.12;  
const float SENSOR2_OFFSET_Z = +0.08;

// Exponential Moving Average (EMA) filter
const float ALPHA = 0.2;  // Smoothing factor (0.1-0.3)
EMAFilter filter1(ALPHA), filter2(ALPHA);

void setup() {
  Serial.begin(115200);  // Match Processing baud rate
  Wire.begin();
  
  // Initialize Sensor 1 (front)
  initAccelerometer(ACCEL_1_ADDR);
  // Initialize Sensor 2 (rear)
  initAccelerometer(ACCEL_2_ADDR);
}

void loop() {
  float z1 = readAccelZ(ACCEL_1_ADDR) + SENSOR1_OFFSET_Z;
  float z2 = readAccelZ(ACCEL_2_ADDR) + SENSOR2_OFFSET_Z;
  
  // Apply noise reduction
  z1 = filter1.update(z1);
  z2 = filter2.update(z2);
  
  // Format: timestamp(ms),sensor1_z,sensor2_z
  Serial.print(millis());
  Serial.print(',');
  Serial.print(z1, 4);  // 4 decimal places
  Serial.print(',');
  Serial.println(z2, 4); 
  
  delay(10);  // ~100Hz sampling
}

// --- HELPER FUNCTIONS ---
void initAccelerometer(uint8_t address) {
  Wire.beginTransmission(address);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // Wake up sensor
  Wire.endTransmission(true);
  
  // Set range to ±8g (0x10) for braking forces
  Wire.beginTransmission(address);
  Wire.write(0x1C);  // ACCEL_CONFIG register
  Wire.write(0x10);  
  Wire.endTransmission(true);
}

float readAccelZ(uint8_t address) {
  Wire.beginTransmission(address);
  Wire.write(0x3F);  // Start at Z-axis high byte
  Wire.endTransmission(false);
  Wire.requestFrom(address, 2, true); 
  
  int16_t raw = (Wire.read() << 8) | Wire.read();
  return raw / 4096.0 * 9.81;  // Convert to m/s² (±8g sensitivity)
}
