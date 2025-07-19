void calibrateSensor(uint8_t address) {
  float sum = 0;
  for(int i=0; i<500; i++) {
    sum += readAccelZ(address);
    delay(5);
  }
  Serial.print("Offset for 0x");
  Serial.print(address, HEX);
  Serial.print(": ");
  Serial.println(-sum/500.0, 4); 
}
