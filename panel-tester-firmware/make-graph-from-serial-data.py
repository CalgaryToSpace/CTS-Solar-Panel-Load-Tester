import pandas as pd
import serial
import serial.tools.list_ports
import csv 
import easygui as g
#Define the serial port and baud rate


def prompt_for_serial_port() -> str:
    ports =  serial.tools.list_ports.comports(include_links=False)
    if len(ports) == 1:
        return ports[0].device
    port_being_used = g.choicebox('Select serial port', 'Serial Port', [port.device for port in ports])
    print("port:", port_being_used)
    return port_being_used


serial_port = prompt_for_serial_port()
baud_rate = 115200
ser = serial.Serial(serial_port, baud_rate)
start_reading = "r2r_value,bus_voltage_V,current_mA,shunt_voltage_mV,shunt_adc" #FIXME make it so that this is a flag that makes the program start reading. 
stop_reading = "Done test_current_at_each_r2r_value()" 
i = 0
stop_flag = 0
start_flag = 0

# Create a CSV file and initialize the CSV writer
csv_file_path = 'Solar_panel_mar26-2024_76.5cm.csv'
csv_file = open(csv_file_path, 'w', newline='')
#csv_writer = csv.writer(csv_file)

try:
    while True:
        if(stop_flag):
            stop_flag = 0
            ser.close()
            csv_file.close()
            break
        # Read data from the serial port
        data_encoded = ser.readline().strip()
        data_decoded = data_encoded.decode()

        if(data_decoded == start_reading):
            start_flag = 1

        if(start_flag):
            while True:
                data_encoded = ser.readline().strip()
                data_decoded = data_encoded.decode()
                if (data_decoded != stop_reading):
                # Write the data to the CSV file
                    csv_file.write(data_decoded)
                    csv_file.write('\n')
                    print(data_decoded)
                else:
                    print("end of test")
                    stop_flag = 1
                    break
        

except KeyboardInterrupt:
    ser.close()  # Close the serial connection when program is interrupted
    csv_file.close()

df = pd.read_csv(csv_file_path) 

print(df)

