from scipy.fftpack import fft, fftfreq
import pyaudio
import matplotlib.pyplot as plt
import numpy as np

CHUNK       = 1024
FORMAT      = pyaudio.paFloat32
CHANNELS    = 1
RATE        = 44100
PAUSE       = 0.0001


# Create audio stream
p = pyaudio.PyAudio()
stream = p.open(
    format              = FORMAT,
    channels            = CHANNELS,
    rate                = RATE,
    input               = True,
    frames_per_buffer   = CHUNK
)


# Create plot for real time visualizations. TODO this will not be needed
fig, ax     = plt.subplots()
x           = np.fft.fftfreq(CHUNK, 1/RATE)[:CHUNK//2]
line_mag,   = ax.plot(x, np.zeros(CHUNK//2))

ax.set_xlim(0, RATE/2)
ax.set_ylim(0, 0.1)


# Continously collect sound from audio stream and process it
while True:
    data = np.frombuffer(stream.read(CHUNK), np.float32)
    yf = fft(data)
    xf = fftfreq(CHUNK, 1/RATE) 

    line_mag.set_ydata(np.abs(yf[:CHUNK//2]))
    fig.canvas.draw()
    plt.pause(PAUSE)
    fig.canvas.flush_events()