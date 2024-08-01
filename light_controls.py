
import time
from rpi_ws281x import *
import argparse

# LED strip configuration:
LED_COUNT      = 144     # Number of LED pixels.
LED_PIN        = 18      # GPIO pin connected to the pixels (18 uses PWM!).
LED_FREQ_HZ    = 800000  # LED signal frequency in hertz (usually 800khz)
LED_DMA        = 10      # DMA channel to use for generating a signal (try 10)
LED_BRIGHTNESS = 100     # Set to 0 for darkest and 255 for brightest
LED_INVERT     = False   # True to invert the signal (when using NPN transistor level shift)
LED_CHANNEL    = 0       # set to '1' for GPIOs 13, 19, 41, 45 or 53
DELAY          = 1


class Lights:

    def __init__(self):
        self.strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, LED_FREQ_HZ, LED_DMA, LED_INVERT, LED_BRIGHTNESS, LED_CHANNEL)
        self.strip.begin()
    

    def frequency_to_color(self, frequency):
        self.clear()

        color = Color(0, 0, 0)

        if frequency >= 300 and frequency < 600:
            color = Color(0, 255, 0)

        elif frequency >= 600 and frequency < 1000:
            color = Color(250, 234, 3)
            
        elif frequency >= 1000 and frequency < 1300:
            color = Color(255, 0, 0)

        else:
            color = Color(0, 0, 255)

        self.strip.setPixelColor(self.strip.getPixels(), color)
        self.strip.show()
        
        # for i in (pixels):
        #     self.strip.setPixelColor(i, color)
        #     self.strip.show()

    
    def clear(self):
        for i in range(self.strip.numPixels()):
            self.strip.setPixelColor(i, Color(0, 0, 0))
            self.strip.show()