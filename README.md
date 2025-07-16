# Cochlear Simulation Model (CSM)

This project was created to visually display and explain how the cochlea translates sound waves into electrical impulses within the inner ear at a high level. This model can be understood by people of any age, background, or level of education. Our goal is to increase access to information about hearing loss and to help those with sensorineural hearing loss understand why they cannot hear as well as they once could.
test


## Running the code

1. Clone the repository onto a Raspberry Pi.

2. Run `make venv`. This will install `portaudio19-dev` for the `pyaudio` package, 
and create a virtual environment and install all dependencies.

3. Plug in your microphone. For this project we are using a Snowball Ice microphone, plugging into the Raspberry Pi 4 via USB.

4. Plug in your LEDs. We are plugging them into pin 18 because it supports PWM. 
Note that we are using an external power supply for the LEDs because the Raspberry Pi cannot 
support the power required by the LEDs.

5. Run `make run`.