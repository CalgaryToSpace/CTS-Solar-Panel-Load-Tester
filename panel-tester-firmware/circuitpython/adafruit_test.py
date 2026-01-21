from adafruit_ina219 import INA219
print("INA219 library loaded successfully!")

import busio
import time
import board

i2c = busio.I2C() 