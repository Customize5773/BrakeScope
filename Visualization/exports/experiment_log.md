### Experiment Log

> # Eddy Current Experiment Log  
> **Last Updated:** 2025-07-19  

## Test Matrix  
| Date | File | Material | Thickness | Field | Velocity | Key Findings |  
|------|------|----------|-----------|-------|----------|--------------|  
| 2025-07-15 | [20250715_Cu_1.5mm_0.8T.csv](raw_data/20250715_Cu_1.5mm_0.8T.csv) | Copper | 1.5mm | 0.8T | 2.1m/s | Peak deceleration -15.2 m/s² |  
| 2025-07-16 | [20250716_Al_2.0mm_0.7T.csv](raw_data/20250716_Al_2.0mm_0.7T.csv) | Aluminum | 2.0mm | 0.7T | 1.8m/s | Energy dissipation 4.0J |  
| 2025-07-17 | [20250717_Steel_1.2mm_0.9T.csv](raw_data/20250717_Steel_1.2mm_0.9T.csv) | Steel | 1.2mm | 0.9T | 1.5m/s | Low-frequency resonance at 7.2Hz |  

## Processed Results  
| Analysis Type | File | Visualization |  
|---------------|------|---------------|  
| Braking Curve | [braking_curves.csv](processed/braking_curves.csv) | ![Braking Plot](plots/braking_curve.png) |  
| Frequency Spectrum | [frequency_spectra.csv](processed/frequency_spectra.csv) | ![Spectrum Plot](plots/frequency_spectrum.png) |  

## Analysis Notes  
### 2025-07-15 Copper Test  
- **Dominant Frequency:** 42.3 Hz  
- **Energy Dissipated:** 5.8J  
- **Deviation from Theory:** +12%  
- **Anomaly:** High-frequency noise above 80Hz - suspected sensor resonance  

### 2025-07-16 Aluminum Test  
```python
# Validation code snippet
theoretical_energy = 0.5 * 0.5 * 1.8**2  # 0.81J
measured_energy = 4.0J
efficiency = measured_energy / theoretical_energy * 100  # 79%
```

## Metadata Standards  
```yaml
naming_convention: "YYYYMMDD_Material_Thickness_Field.csv"  
required_metadata:  
  - MATERIAL  
  - THICKNESS_MM  
  - FIELD_T  
  - VELOCITY_MS  
  - TEMPERATURE_C  
data_columns:  
  raw: [timestamp, sensor1_raw, sensor2_raw]  
  processed: [time, deceleration_front, velocity, energy]  
```

## Change Log  
| Date | Version | Changes |  
|------|---------|---------|  
| 2025-07-19 | 1.2 | Added phase data to spectral analysis |  
| 2025-07-18 | 1.1 | Improved velocity integration algorithm |  
| 2025-07-15 | 1.0 | Initial logging standard |  
```
