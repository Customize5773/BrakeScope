import pandas as pd
import numpy as np
from scipy import integrate, signal

def process_braking_curve(input_file):
    df = pd.read_csv(input_file)
    
    # Calculate deceleration (remove gravity)
    df['deceleration_front'] = df['sensor1_filtered'] + 9.81
    df['deceleration_rear'] = df['sensor2_filtered'] + 9.81
    
    # Integrate to velocity
    t = df['timestamp'] / 1000
    df['velocity'] = 1.8 - integrate.cumtrapz(df['deceleration_front'], t, initial=0)
    
    # Calculate energy
    kinetic_energy = 0.5 * 0.5 * df['velocity']**2  # m=0.5kg
    df['energy_dissipated'] = kinetic_energy.iloc[0] - kinetic_energy
    
    # Save processed
    df[['time', 'deceleration_front', 'deceleration_rear', 'velocity', 'energy_dissipated']]\
      .to_csv("braking_curves.csv", index=False)

def process_spectrum(input_file):
    df = pd.read_csv(input_file)
    t = df['timestamp'] / 1000
    fs = 1 / np.mean(np.diff(t))
    
    # Compute FFT
    for sensor in ['sensor1', 'sensor2']:
        x = df[sensor + '_filtered'].values
        f, Pxx = signal.welch(x - np.mean(x), fs, nperseg=1024)
        df[sensor + '_amplitude'] = np.sqrt(Pxx)
        df[sensor + '_phase'] = np.angle(np.fft.fft(x))[:len(f)] * 180/np.pi
        
    pd.DataFrame({
        'frequency': f,
        'amplitude_front': df['sensor1_amplitude'],
        'phase_front': df['sensor1_phase'],
        'amplitude_rear': df['sensor2_amplitude'],
        'phase_rear': df['sensor2_phase']
    }).to_csv("frequency_spectra.csv", index=False)
