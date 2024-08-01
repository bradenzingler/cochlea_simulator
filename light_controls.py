from rpi_ws281x import *
import math

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
    
    def pos_to_pixels(self, pos_in_mm):
        """
        Map the basiliar membrane position (in mm) to LED strip pixels.
        """
        pixels = int((pos_in_mm / BM_LENGTH) * LED_COUNT)
        return max(0, min(LED_COUNT, pixels))

    def freq_to_pos(self, frequency: float):
        """
        Convert frequency to position (mm) using the Greenwood function.

        Reference: Greenwood D. "A cochlear frequency-position for several
        species - 29 years later", J. Acoust. Soc Am. 1990(87) pp2592-2605.
        """
        a = 2.1
        A = 165.4
        pos_in_mm = math.log10((frequency/A)+K) * (BM_LENGTH/a)
        return pos_in_mm

    def frequency_to_strip(self, frequency):
        """
        Light up strip based on frequency.
        """
        self.clear()
        pos = self.freq_to_pos(frequency=frequency)
        pixels = self.pos_to_pixels(pos_in_mm=pos)
        for i in (pixels):
            self.strip.setPixelColor(i, Color(255, 255, 255))
        self.strip.show()
    
    def clear(self):
        """
        Set all lights to off.
        """
        for i in range(self.strip.numPixels()):
            self.strip.setPixelColor(i, Color(0, 0, 0))
            self.strip.show()

if __name__ == "__main__":
    lights = Lights()
    pos = lights.frequency_to_strip(frequency=1000)
    print(pos)