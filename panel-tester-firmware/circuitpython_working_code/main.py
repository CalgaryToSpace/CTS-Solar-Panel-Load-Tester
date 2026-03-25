import time # Standard Python module used for sleep()
import board # CircuitPython hardware definition module for pin definitions
import busio # CircuitPython I2C support
from adafruit_ina219 import INA219
import digitalio

#--------------- Function definitions ---------
def init_dac_pins():
    #function to initialize pins
    # LSB (bit 0) to MSB (bit 7)
    pin_list = [
        board.GP6,   # bit 0 (LSB)
        board.GP7,   # bit 1
        board.GP8,   # bit 2
        board.GP9,   # bit 3
        board.GP10,  # bit 4
        board.GP11,  # bit 5
        board.GP12,  # bit 6
        board.GP13   # bit 7 (MSB)
    ]

    dac_pins = []

    for pin in pin_list:
        dac = digitalio.DigitalInOut(pin)
        dac.direction = digitalio.Direction.OUTPUT
        dac_pins.append(dac)

    return dac_pins

def write_dac(dac_pins, value):
    """
    Writes an 8-bit integer (0–255) to the DAC pins.
    dac_pins[0] = LSB
    dac_pins[7] = MSB
    """
    # Clamp value to valid 8-bit range
    value = max(0, min(255, value))
    for bit in range(8):
        dac_pins[bit].value = (value >> bit) & 0x01

def ramp(res, freq):
    #creates a ramp function given a resolution and a frequency
    #reads voltage from INA
    #prints values (quick debugging purposes)
    #writes to a file 
    delay = 1/ (res*freq)
    for i in range(256):
        write_dac(dac, i)
        # Read INA219 measurements
        bus_v = ina219.bus_voltage           # V
        shunt_v = ina219.shunt_voltage       # mV
        current_mA = ina219.current 		#mA
        
        print(f"{bus_v:.2f},{shunt_v:.2f},{current_mA:.2}")
        if ina219.overflow:
            print("Internal Math Overflow Detected!")
            print("")
        
        time.sleep(delay) 

# ----------- setup -----------------

print("INA219 library loaded successfully!")

# Make sure load resistance is connected
# Do not use pins 27 and 26 of picoboard (GP21 and GP20)
i2c_bus = busio.I2C(scl=board.GP19, sda=board.GP18)
print("ina219 connected")

# INA219 class constructor object
ina219 = INA219(i2c_bus)
print("constructor object")

# Set calibration to 16V 400mA to maximize resolution.
# This sets the ADC configuration to 12-bit, 1 sample, continuous mode
ina219.set_calibration_16V_400mA()

#init pins
dac = init_dac_pins()


# ramp
STEPS = 200           # ramp resolution
SAW_FREQ = 1          # 1 Hz sawtooth wave
DELAY = 1 / (STEPS * SAW_FREQ)

# measure and display loop
while True:
   print("bus_voltage_(V), Shunt_voltage_(mV), current_mA")
   ramp(STEPS, SAW_FREQ)
   print("\n")
   time.sleep(1) # a delay after each sawtooth`12