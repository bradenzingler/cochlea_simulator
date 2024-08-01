from rpi_ws281x import *
import math
import numpy as np

################################# GLOBALS ######################################
LED_COUNT      = 144     # Number of LED pixels.
LED_PIN        = 18      # GPIO pin connected to the pixels (18 uses PWM!).
LED_FREQ_HZ    = 800000  # LED signal frequency in hertz (usually 800khz)
LED_DMA        = 10      # DMA channel to use for generating a signal (try 10)
LED_BRIGHTNESS = 100     # Set to 0 for darkest and 255 for brightest
LED_INVERT     = False   # True to invert the signal 
LED_CHANNEL    = 0       # Set to '1' for GPIOs 13, 19, 41, 45 or 53
BM_LENGTH      = 1000    # Length of basiliar membrane (mm)
K              = 0.88    # Greenwood integration constant
SMALL_A        = 2.1     # Slope of the straight-line portion of freq-pos curve
BIG_A          = 165.4   # Scale const between characteristic freq + upper freq

class Lights:
    def __init__(self):
        self.strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, LED_FREQ_HZ, LED_DMA, LED_INVERT, LED_BRIGHTNESS, LED_CHANNEL)
        self.strip.begin()
        self.previous_pixels = np.zeros(LED_COUNT, dtype=bool)
        self.current_colors = [Color(0, 0, 0)] * LED_COUNT
    
    def pos_to_pixels(self, pos_in_mm):
        """
        Map the basiliar membrane position (in mm) to LED strip pixels.
        """
        pixels = int((pos_in_mm / BM_LENGTH) * LED_COUNT)
        return max(0, min(LED_COUNT-1, pixels))

    def freq_to_pos(self, frequency: float) -> float:
        """
        Convert frequency to position (mm) using the Greenwood function.

        Reference: Greenwood D. "A cochlear frequency-position for several
        species - 29 years later", J. Acoust. Soc Am. 1990(87) pp2592-2605.
        """
        return math.log10((frequency / BIG_A) + K) * (BM_LENGTH / SMALL_A)

    def frequency_to_color(self, frequency: int) -> None:
        return Color(255, 255, 255)

    def frequency_to_strip(self, frequency: float) -> None:
        """
        Light up strip based on frequency value.
        """
        pos         = self.freq_to_pos(frequency=frequency)
        pixels      = self.pos_to_pixels(pos_in_mm=pos)
        color       = self.frequency_to_color(frequency=frequency)
        new_colors  = [Color(0, 0, 0)] * LED_COUNT

        for i in range(pixels):
            new_colors[i] = color
        
        if new_colors != self.current_colors:
            for i in range(LED_COUNT):
                self.strip.setPixelColor(i, new_colors[i])
            self.strip.show()
            self.current_colors = new_colors
    
    def clear(self):
        """
        Set all lights to off.
        """
        for i in range(self.strip.numPixels()):
            self.strip.setPixelColor(i, Color(0, 0, 0))
        self.strip.show()