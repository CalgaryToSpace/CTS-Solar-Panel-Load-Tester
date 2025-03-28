# mpremote connect <port_path> mip install github:josverl/micropython-stubs/mip/typing.mpy

from typing import Literal

from machine import ADC, I2C, Pin
from neopixel import NeoPixel
import time
import json

from ina219 import INA219

INA_SHUNT_OMHS = 0.100  # R100 = 0.1 ohms
ina_i2c = I2C(1, scl=Pin(15), sda=Pin(14), freq=100_000)
ina: INA219  # Constructed/initialized in `init_ina()`

ADC_PIN = ADC(Pin(29))
# Conversion factor for RP2040 ADC (12-bit resolution, 3.3V reference)
ADC_CONVERSION_FACTOR = 3.3 / 65535
ADC_VOLTAGE_DIVIDER_RATIO = 4.7 / (4.7 + 18)  # 4k7 bottom, 18k top

ONBOARD_LED_PIN = Pin(16, Pin.OUT)


def init_ina() -> None:
    """Initialize INA219 current sensor. Perform I2C scan."""
    print("Scanning I2C bus for INA219.")
    i2c_addr_list: list[int] = ina_i2c.scan()
    print(f"Found {len(i2c_addr_list)} devices: {i2c_addr_list}")

    if i2c_addr_list != [0x40]:
        raise ValueError("INA219 not found at expected address.")

    global ina
    ina = INA219(ina_i2c, addr=0x40)
    ina.set_calibration_32V_2A()


def init() -> None:
    print("Starting init.")
    init_ina()
    print("Init complete.")


def reset() -> None:
    # CLI alias.
    init()


def read_adc_voltage() -> float:
    """Reads the voltage from the ADC pin, of the solar panel.

    Reverses the voltage divider calculation.
    """
    raw_value = ADC_PIN.read_u16()
    voltage = raw_value * ADC_CONVERSION_FACTOR / ADC_VOLTAGE_DIVIDER_RATIO
    return voltage


def log_ina_json(
    timestamp_ms: int | None = None,
    *,
    enable_fields: tuple[
        Literal["current_mA", "bus_voltage_mV", "shunt_voltage_mV", "adc_voltage_mV"],
        ...,
    ] = ("current_mA", "bus_voltage_mV", "adc_voltage_mV"),
) -> None:
    shunt_mV = ina.shunt_voltage * 1000

    data = {}

    if "current_mA" in enable_fields:
        data["current_mA"] = shunt_mV / INA_SHUNT_OMHS
    if "bus_voltage_mV" in enable_fields:
        data["bus_voltage_mV"] = ina.bus_voltage
    if "shunt_voltage_mV" in enable_fields:
        data["shunt_voltage_mV"] = shunt_mV
    if "adc_voltage_mV" in enable_fields:
        data["adc_voltage_mV"] = read_adc_voltage()

    if timestamp_ms is not None:
        data["timestamp_ms"] = timestamp_ms

    print(json.dumps(data))


def sleep_ms_and_log_ina_json(sleep_time_ms: int, log_period_ms: int = 250) -> None:
    start_time_ms = time.ticks_ms()

    while True:
        current_time_ms = time.ticks_ms()
        elapsed_time_ms = time.ticks_diff(current_time_ms, start_time_ms)

        if elapsed_time_ms >= sleep_time_ms:
            break  # Stop when the total sleep time has elapsed

        log_ina_json(elapsed_time_ms)

        # Calculate the remaining time
        next_log_time_ms = time.ticks_add(
            start_time_ms, elapsed_time_ms + log_period_ms
        )
        remaining_time_ms = time.ticks_diff(next_log_time_ms, time.ticks_ms())

        if remaining_time_ms > 0:
            time.sleep_ms(min(remaining_time_ms, sleep_time_ms - elapsed_time_ms))


def sleep_ms_and_get_ina_stats(sleep_time_ms: int) -> dict[str, float]:
    start_time_ms = time.ticks_ms()
    current_values_mA = []
    while True:
        current_values_mA.append(ina.shunt_voltage * 1000 / INA_SHUNT_OMHS)
        current_time_ms = time.ticks_ms()
        elapsed_time_ms = time.ticks_diff(current_time_ms, start_time_ms)

        if elapsed_time_ms >= sleep_time_ms:
            break

    return {
        "min": min(current_values_mA),
        "max": max(current_values_mA),
        "avg": sum(current_values_mA) / len(current_values_mA),
        "len": len(current_values_mA),
    }


def print_available_commands() -> None:
    print("""
Available commands:
    - exit()  # Doesn't really do anything.
    - init() AKA reset()
    - read_adc_voltage
    - log_ina_json()
    - sleep_ms_and_log_ina_json(sleep_time_ms: int, log_period_ms: int = 250)
          
    """)


def help() -> None:
    print_available_commands()

def set_led_color(r: int, g: int, b: int) -> None:
    """Display a color on the LED.
    Values should be between 0 and 255.
    """
    np = NeoPixel(ONBOARD_LED_PIN, 1)
    np[0] = (r, g, b)
    np.write()

def prompt_and_execute() -> None:
    print("Enter a command, or use 'help':")
    command = input(">> ").strip()

    if command == "exit":
        print("Exiting.")
        return

    # If the command does not have parentheses, add them.
    if "(" not in command and ")" not in command:
        command += "()"

    print(f"Executing command: {command}\n")

    try:
        exec(command)
    except Exception as e:
        print(f"Error: {e}")
    print()


def main() -> None:
    init()

    PERIOD_MS = 500

    while 1:
        # prompt_and_execute()
        log_ina_json()

        if ina.bus_voltage < 0.5:
            set_led_color(255, 0, 0)
        else:
            set_led_color(0, 0, 255)
            
        time.sleep_ms(50)
        set_led_color(0, 0, 0)
        time.sleep_ms(PERIOD_MS - 50)


while True:
    main()
