File logFile;

bool initSD(int csPin) {
  return SD.begin(csPin);
}

void createNewFile() {
  String filename = generateFilename();
  logFile = SD.open(filename, FILE_WRITE);
  if (logFile) {
    logFile.println("timestamp,z1,z2");  // CSV header
  }
}

void writeBufferToSD(SensorData* buffer, int count) {
  for (int i = 0; i < count; i++) {
    logFile.print(buffer[i].timestamp);
    logFile.print(',');
    logFile.print(buffer[i].z1, 4);
    logFile.print(',');
    logFile.println(buffer[i].z2, 4);
  }
  logFile.flush();  // Ensure data is written
}
