import time
import board
import pwmio
import busio
from adafruit_ina219 import INA219

# ------------------------------------------
# PWM SETUP (controls MOSFET gate)
# ------------------------------------------
pwm = pwmio.PWMOut(
    board.GP0,          # PWM output pin
    frequency=50000,    # 50 kHz PWM (Change for desired frequency
    duty_cycle=0
)

# ------------------------------------------
# INA219 SETUP (measures load current)
# ------------------------------------------
i2c = busio.I2C(board.GP5, board.GP4)  # GP5=SCL, GP4=SDA
ina = INA219(i2c)

ina.bus_adc_resolution = INA219.ADCRES_12BIT_32S
ina.shunt_adc_resolution = INA219.ADCRES_12BIT_32S

# ------------------------------------------
# LOG FILE SETUP
# ------------------------------------------
filename = "data_log.txt"
log = open(filename, "w")

log.write("time_s, duty_cycle, bus_voltage_V, shunt_voltage_mV, current_mA\n")
log.flush()

# ------------------------------------------
# PWM SAWTOOTH SETTINGS
# ------------------------------------------
STEPS = 200           # ramp resolution
SAW_FREQ = 1          # 1 Hz sawtooth wave
DELAY = 1 / (STEPS * SAW_FREQ)

# ------------------------------------------
# MAIN LOOP
# ------------------------------------------
start_time = time.monotonic()

while True:
    for i in range(STEPS):
        # Set PWM duty (0–65535)
        duty = int((i / (STEPS - 1)) * 65535)
        pwm.duty_cycle = duty

        # Read INA219 measurements
        bus_v = ina.bus_voltage           # V
        shunt_v = ina.shunt_voltage       # mV
        #current = ina.current             # mA

        t = time.monotonic() - start_time

        # Write one line of data
        log.write(f"{t:.3f}, {duty}, {bus_v:.4f}\n")
        log.flush()

        time.sleep(DELAY)
