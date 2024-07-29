import numpy as np
from scipy.fftpack import fft, fftfreq
import pyaudio
import matplotlib.pyplot as plt

CHUNK = 1024
FORMAT = pyaudio.paFloat32
CHANNELS = 1
RATE = 44100
PAUSE = 0.0001
NUM_BINS = 100

# Create audio stream
p = pyaudio.PyAudio()

def open_stream():
    return p.open(
        format=FORMAT,
        channels=CHANNELS,
        rate=RATE,
        input=True,
        frames_per_buffer=CHUNK
    )

stream = open_stream()

# Create plot for real time visualizations. TODO this will not be needed
fig, ax = plt.subplots()
x = np.linspace(0, RATE / 2, NUM_BINS)
bars = ax.bar(x, np.zeros(NUM_BINS), width=RATE/(2*NUM_BINS))

ax.set_xlim(0, RATE / 2)
ax.set_ylim(0, 1)

bin_edges = np.linspace(0, CHUNK // 2, NUM_BINS + 1, dtype=int)

# Continuously collect sound from audio stream and process it
try:
    while True:
        try:
            if not stream.is_active():
                stream = open_stream()
            data = np.frombuffer(stream.read(CHUNK, exception_on_overflow=False), np.float32)
        except IOError as e:
            # Handle buffer overflow or stream errors
            print(f"Error reading audio stream: {e}")
            continue

        yf = fft(data)
        magnitudes = np.abs(yf[:CHUNK // 2])
        binned_magnitudes = [np.sum(magnitudes[bin_edges[i]:bin_edges[i + 1]]) for i in range(NUM_BINS)]

        # Check if max is zero to avoid division by zero
        max_magnitude = np.max(binned_magnitudes)
        if max_magnitude == 0:
            normalized_magnitudes = binned_magnitudes
        else:
            normalized_magnitudes = binned_magnitudes / max_magnitude

        for bar, height in zip(bars, normalized_magnitudes):
            bar.set_height(height)
        
        fig.canvas.draw()
        plt.pause(PAUSE)
        fig.canvas.flush_events()
except KeyboardInterrupt:
    pass
finally:
    stream.stop_stream()
    stream.close()
    p.terminate()
