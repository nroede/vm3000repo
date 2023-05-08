# Author: Nils Roede
# Based on code from https://learnpython.com/blog/plot-waveform-in-python/

import wave
import numpy as np
import matplotlib.pyplot as plt

# Get wav file information
wav_obj = wave.open(r'C:\Users\nils\OneDrive - Duke University\Documents\GRL Research\Recording.wav', 'rb')
sample_freq = wav_obj.getframerate()
print("sample_freq =", sample_freq)
n_samples = wav_obj.getnframes()
print("n_samples =", n_samples)
t_audio = n_samples/sample_freq
print("t_audio =", t_audio)
n_channels = wav_obj.getnchannels()
print("n_channels =", n_channels)

# Split data into L and R channels
signal_wave = wav_obj.readframes(n_samples)
signal_array = np.frombuffer(signal_wave, dtype=np.int16)
l_channel = signal_array[0::2]
r_channel = signal_array[1::2]

# Plot L channel waveform
times = np.linspace(0, t_audio, num=int(n_samples/2))
plt.figure(figsize=(15, 5))
plt.plot(times, l_channel)
plt.title('Left Channel')
plt.ylabel('Signal Value')
plt.xlabel('Time (s)')
plt.xlim(0, t_audio)
plt.show()

# Plot L channel spectrogram
plt.figure(figsize=(15, 5))
plt.specgram(l_channel, Fs=sample_freq, vmin=-20, vmax=50)
plt.title('Left Channel')
plt.ylabel('Frequency (Hz)')
plt.xlabel('Time (s)')
plt.xlim(0, int(t_audio/2))
plt.colorbar()
plt.show()