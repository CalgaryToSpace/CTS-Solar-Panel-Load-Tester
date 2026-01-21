import board
import digitalio
import time

# Set up pin GP13 as an output
yellow = digitalio.DigitalInOut(board.GP13)
yellow.direction = digitalio.Direction.OUTPUT

# Turn the LED on for 20 seconds, then off
yellow.value = True   # LED ON
time.sleep(20)
yellow.value = False  # LED OFF
