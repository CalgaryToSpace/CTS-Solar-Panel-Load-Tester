
import digitalio
import board
import time

#functions

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
#         bus_v = ina219.bus_voltage           # V
#         shunt_v = ina219.shunt_voltage       # mV
#         
#         print("Bus Voltage:   " + "{:.2f}".format(bus_v) + " V")
#         print("Shunt Voltage: " + "{:.2f}".format(shunt_v) + " mV")
#         if ina219.overflow:
#             print("Internal Math Overflow Detected!")
#             print("")
        
        time.sleep(delay) 
  
#put this in main code
dac = init_dac_pins()

# ramp
STEPS = 200           # ramp resolution
SAW_FREQ = 1          # 1 Hz sawtooth wave
DELAY = 1 / (STEPS * SAW_FREQ)



# measure and display loop
while True:
   print("in loop")
   ramp(STEPS, SAW_FREQ)
   #time.sleep(1) # a delay after each sawtooth`12
    