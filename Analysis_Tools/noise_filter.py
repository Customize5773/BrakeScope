#!/usr/bin/env python3
"""
Eddy Current Data Filtering Tool
Applies Butterworth filters to suppress noise while preserving braking signatures
"""

import numpy as np
import pandas as pd
from scipy.signal import butter, filtfilt
import matplotlib.pyplot as plt

def butter_lowpass(cutoff, fs, order=5):
    """Design Butterworth low-pass filter"""
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return b, a

def apply_filter(data, cutoff, fs, order=5):
    """Apply zero-phase filter to preserve timing"""
    b, a = butter_lowpass(cutoff, fs, order=order)
    return filtfilt(b, a, data)

def main(input_file, output_file, cutoff_hz=25):
    # Load experimental data
    df = pd.read_csv(input_file)
    t = df['timestamp'] / 1000  # Convert ms to seconds
    fs = 1 / np.mean(np.diff(t))  # Calculate sampling rate
    
    # Process both sensors
    for sensor in ['sensor1', 'sensor2']:
        raw = df[f'{sensor}_raw'].values
        
        # Apply filter
        filtered = apply_filter(raw, cutoff_hz, fs)
        df[f'{sensor}_filtered'] = filtered
        
        # Plot comparison
        plt.figure(figsize=(12, 4))
        plt.plot(t, raw, 'b-', alpha=0.5, label='Raw')
        plt.plot(t, filtered, 'r-', label=f'Filtered ({cutoff_hz}Hz)')
        plt.title(f'{sensor} Acceleration')
        plt.xlabel('Time (s)')
        plt.ylabel('Acceleration (m/s²)')
        plt.legend()
        plt.savefig(f"{output_file}_{sensor}.png")
    
    # Save filtered data
    df.to_csv(output_file, index=False)
    print(f"Filtered data saved to {output_file}")

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description='Filter accelerometer noise')
    parser.add_argument('input', help='Input CSV file')
    parser.add_argument('output', help='Output CSV file')
    parser.add_argument('--cutoff', type=float, default=25, 
                        help='Low-pass cutoff frequency (Hz)')
    args = parser.parse_args()
    
    main(args.input, args.output, args.cutoff)
