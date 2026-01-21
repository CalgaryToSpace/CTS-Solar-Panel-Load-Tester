import time # Standard Python module used for sleep()
import board # CircuitPython hardware definition module for pin definitions
import busio # CircuitPython I2C support
from adafruit_ina219 import INA219
import digitalio


print("INA219 library loaded successfully!")

# Make sure load resistance is connected
# Do not use pins 27 and 26 of picoboard (GP21 and GP20)
i2c_bus = busio.I2C(scl=board.GP19, sda=board.GP18)
print('ina219 connected')

# INA219 class constructor object
ina219 = INA219(i2c_bus)
print("constructor object")

# Set calibration to 16V 400mA to maximize resolution.
# This sets the ADC configuration to 12-bit, 1 sample, continuous mode
ina219.set_calibration_16V_400mA()


print("DC Voltage, Current, and Power:")
# measure and display loop
while True:
    # Read values
    bus_voltage_V = ina219.bus_voltage # voltage in V on the load side V-
    shunt_voltage_V = ina219.shunt_voltage # voltage in V across the shunt V+ and V-
    current_mA = ina219.current # current in mA
    power_W = ina219.power # power in Watts



# Compute load voltage
    loadVoltage_V = shunt_voltage_V + bus_voltage_V
    # Convert shunt voltage from Volts to milliVolts
    shunt_voltage_mV = shunt_voltage_V / 1000
    # Convert power from Watts to milliWatts
    power_mW = power_W / 1000

    # Send data to computer console over USB
    # Output values to 2 decimal place
    
    
    print("Bus Voltage:   " + "{:.2f}".format(bus_voltage_V) + " V")
    print("Shunt Voltage: " + "{:.2f}".format(shunt_voltage_mV) + " mV")
    print("Load Voltage:  " + "{:.2f}".format(loadVoltage_V) + " V")
    print("Current:       " + "{:.2f}".format(current_mA) + " mA")
    print("Power:         " + "{:.2f}".format(power_mW) + " mW")
    # Check internal calculations haven't overflowed (doesn't detect ADC overflows)
    if ina219.overflow:
        print("Internal Math Overflow Detected!")
    print("")
    # delay a second
    time.sleep(1)