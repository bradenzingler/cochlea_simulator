#!/usr/bin/env python
import numpy as np
from scipy.fftpack import fft
import pyaudio
from light_controls import Lights
import time

#### AUDIO CONFIG ##############################################################
CHUNK       = 1024                  # Number of btytes to read in per buffer
FORMAT      = pyaudio.paFloat32     # Audio format
CHANNELS    = 1                     # Number of audio channels to record in
RATE        = 44100                 # Sampling rate
NUM_BINS    = 1500                  # Bins to split frequency into

def open_stream(p):
    """
    Open the audio stream.
    """
    return p.open(
        format              = FORMAT,
        channels            = CHANNELS,
        rate                = RATE,
        input               = True,
        frames_per_buffer   = CHUNK
        )

def read_audio(stream, frequencies, lights, bin_edges, p):
    """
    Read audio input from the opened audio stream.
    """
    try:
        while True:
            data = np.frombuffer(
                stream.read(CHUNK, 
                exception_on_overflow=False), 
                np.float32
                )
            yf = fft(data)
            magnitudes = np.abs(yf[:CHUNK // 2])
            binned_magnitudes = [np.sum(magnitudes[bin_edges[i]:bin_edges[i + 1]]) for i in range(NUM_BINS)]
            max_magnitude = np.max(binned_magnitudes)

            if max_magnitude == 0:
                normalized_magnitudes = binned_magnitudes
            else:
                normalized_magnitudes = binned_magnitudes / max_magnitude
                
            idx = np.argmax(normalized_magnitudes)
            freq = frequencies[bin_edges[idx]]
            lights.frequency_to_strip(frequency=freq)

    except KeyboardInterrupt:
        # Allow keyboard to cancel the program
        pass

    finally:
        print("Exiting...")
        lights.clear()
        stream.stop_stream()
        stream.close()
        p.terminate()

def start():
    """
    Initialize and begin stream.
    """
    p = pyaudio.PyAudio()
    stream = open_stream(p)
    lights = Lights()
    bin_edges = np.linspace(0, CHUNK // 2, NUM_BINS + 1, dtype=int)
    frequencies = np.linspace(0, RATE / 2, CHUNK // 2)
    read_audio(
        stream      = stream, 
        frequencies = frequencies, 
        lights      = lights,
        bin_edges   = bin_edges,
        p           = p
    )

start()