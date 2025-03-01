import serial
import pandas as pd
import matplotlib.pyplot as plt
import easygui as g

def prompt_for_serial_port() -> str:
    ports = serial.tools.list_ports.comports()
    return g.choicebox('Select serial port', 'Serial Port', [port.device for port in ports])

# Function to read data from serial port
def read_serial_data(serial_port: str) -> list[list[str]]:
    ser = serial.Serial(serial_port, baudrate=115200)
    data = []
    try:
        while True:
            line = ser.readline().decode().strip()
            if line:
                data.append(line.split(','))
    except KeyboardInterrupt:
        ser.close()
        return data

while 1:
    # Prompt user for filename
    filename = input("Enter filename to save data and graph: ")

    # Prompt user for serial port
    serial_port = input("Enter serial port (e.g., COM1, /dev/ttyUSB0): ")

    # Read data from serial port
    data = read_serial_data(serial_port)

    # Create DataFrame from data
    df = pd.DataFrame(data, columns=['dac_value', 'bus_voltage_V', 'current_mA', 'shunt_voltage_mV', 'shunt_adc'])

    # Convert columns to numeric
    df = df.apply(pd.to_numeric)

    # Plot graph
    plt.scatter(df['bus_voltage_V'], df['current_mA'])
    plt.xlabel('Bus Voltage (V)')
    plt.ylabel('Current (mA)')
    plt.title('Current vs Bus Voltage')
    plt.grid(True)
    plt.savefig(filename + '_graph.png')
    plt.show()

    # Save raw data to file
    df.to_csv(filename + '_data.csv', index=False)

