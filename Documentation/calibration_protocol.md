# Accelerometer Calibration Protocol  

```markdown
# Accelerometer Calibration Protocol  
Proper calibration is essential for accurate braking force measurements. Follow this procedure before each experimental session.  

---

## Equipment Required  
1. Calibration jig (flat, rigid platform)  
2. Digital inclinometer (±0.1° accuracy)  
3. Known mass (e.g., 100g weight)  
4. Arduino with installed `accelerometer_reader` firmware  

---

## Step 1: Sensor Positioning  
1. Mount both accelerometers on the calibration jig:  
   ```plaintext
   [Sensor Front]      [Sensor Rear]
   ┌─────────────┐     ┌─────────────┐
   │    Z ↑      │     │    Z ↑      │
   └─────────────┘     └─────────────┘
   ```  
2. Ensure:  
   - Z-axis perpendicular to mounting surface  
   - No magnetic materials within 15cm  
   - USB cable not straining connections  

---

## Step 2: Gravity-Based Calibration  
### Static Offset Calculation  
Run the calibration script included in `sensor_calibration.h`:  
```cpp
void calibrateSensors() {
  Serial.println("Beginning calibration...");
  delay(2000);  // Allow stabilization
  
  for (uint8_t addr : {0x68, 0x69}) {
    float sum = 0;
    for(int i=0; i<500; i++) {
      sum += readAccelZ(addr);  // Raw Z-value
      delay(10);
    }
    float offset = -sum/500.0 - 9.80665;  // Expected gravity
    Serial.print("SENSOR 0x");
    Serial.print(addr, HEX);
    Serial.print(" OFFSET = ");
    Serial.println(offset, 4);
  }
}
```

**Expected Output:**  
```
SENSOR 0x68 OFFSET = -0.1243
SENSOR 0x69 OFFSET = +0.0876
```

### Apply Offsets  
Update `sensor_calibration.h` with measured values:  
```cpp
// Front sensor (0x68)
const float SENSOR1_OFFSET_Z = -0.1243;  

// Rear sensor (0x69)
const float SENSOR2_OFFSET_Z = +0.0876; 
```

---

## Step 3: Dynamic Sensitivity Verification  
1. Connect calibration mass to jig with string:  
   ```plaintext
     [Jig] ----- string ----- [100g mass]
   ```  
2. Lift mass 10cm and release freely  
3. In Processing visualization:  
   - Confirm both sensors show:  
     ```plaintext
     Release:  0.0g → +1.0g (impact)  
     Rebound: +1.0g → -0.5g (oscillation)  
     ```  
4. Acceptable error: ±0.05g (0.5 m/s²)  

---

## Step 4: In-Situ Vehicle Calibration  
After mounting on car:  
1. Place car on level track section  
2. Run `static_offset` mode in Processing:  
   ```processing
   void keyPressed() {
     if (key == 'c') {
       float sum1 = 0, sum2 = 0;
       for(int i=0; i<200; i++) {
         sum1 += values[0][width-1];
         sum2 += values[1][width-1];
         delay(10);
       }
       DEFAULT_CENTER = (sum1/200 + sum2/200) / 2;
     }
   }
   ```  
   Press 'c' to capture new gravity center  

---

## Calibration Validation Table  
| Test Case          | Expected Result        | Pass Criteria       |  
|--------------------|------------------------|---------------------|  
| Static horizontal  | Z = -9.81 ±0.05 m/s²   | Both sensors match |  
| 30° incline        | Z = -8.50 ±0.10 m/s²   | Math: -9.81*cos(30°)|  
| Quick acceleration | Peak > +1.5g           | Symmetric response |  
| Free fall (0.5s)   | Z ≈ 0.0 m/s²           | Duration < 100ms error |  

---

## Troubleshooting  
**Problem**: Consistent offset after calibration  
**Solution**:  
1. Check for mechanical stress on sensor casing  
2. Verify no temperature gradients (>3°C change requires recalibration)  
3. Test with alternative I²C bus (e.g., separate Arduino)  

**Problem**: Asymmetric dynamic response  
**Solution**:  
1. Apply mechanical damping putty to sensor base  
2. Ensure sampling rate matches in firmware (100Hz) and Processing  
3. Replace USB cable (poor shielding causes noise)  

---

## Calibration Frequency  
| Condition                     | Recalibrate |  
|-------------------------------|-------------|  
| Temperature change > 5°C      | ✓ Required  |  
| After mechanical impact       | ✓ Required  |  
| New mounting position         | ✓ Required  |  
| 30 days since last calibration| ✓ Recommended |  
| Same session, same environment| ✗ Not needed |  

> **Pro Tip**: Store calibration constants in EEPROM using:  
> ```cpp
> EEPROM.put(0, SENSOR1_OFFSET_Z);
> EEPROM.put(4, SENSOR2_OFFSET_Z);
> ```

---

## Next Steps  
1. Upload calibrated firmware to Arduino  
2. Proceed to [Experimental Procedure](../experimental_procedure.md)
