### Hardware Requirements  
1. **Components**:  
   - SD card module (SPI interface)  
   - MicroSD card (formatted FAT32)  
   - Real-Time Clock (RTC) module (optional for precise timing)  

2. **Wiring**:  
```markdown
Arduino -> SD Module
-------------------
5V     -> VCC
GND    -> GND
D13    -> SCK
D12    -> MISO
D11    -> MOSI
D10    -> CS (Chip Select)
```

---

### Usage Workflow  
1. Insert SD card and power on  
2. Data logs to `LOG_<timestamp>.csv`  
3. Remove SD card after experiment  
4. Load CSV in Processing for analysis:  
```processing
void loadCSV(String filename) {
  String[] rows = loadStrings(filename);
  for (int i=1; i<rows.length; i++) {  // Skip header
    float[] vals = float(split(rows[i], ','));
    pushZValue(new float[]{vals[1], vals[2]});
  }
}
```

> **Note**: This module can be merged with the main accelerometer reader by adding `#define ENABLE_SD_LOGGING` compile-time flag for flexibility.
