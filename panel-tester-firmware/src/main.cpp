#include "MCP_DAC.h"
#include "SPI.h"
#include "INA219.h"

MCP4921 MCP(11, 13); // Initialize DAC with MOSI=D11, CLK=D13

// MCP4921 MCP;  //  HW SPI, Default: MOSI=D11, CLK=D13

INA219 INA(0x40); // Initialize INA219 sensor at I2C address 0x40

void voltage_loop(); // Varies voltage on pin A1 to increment from 0 V to 3.3 V
float analog_to_voltage(); // Converts analogRead() value to physical voltage
void INA_measurements(); // Measures bus voltage and input current through R_SHUNT3

void setup() {  
  Serial.begin(115200);
  Serial.println(__FILE__);
  // SPI.begin(); // Use with HW SPI only

  MCP.begin(10); // Set pin 10 to OUTPUT and HIGH
  if (!INA.begin()) (Serial.print("INA219 sensor not connected."));
  
  Serial.print("MCP_DAC_LIB_VERSION: "); Serial.println(MCP_DAC_LIB_VERSION); Serial.println();
  Serial.print("CHANNELS:\t"); Serial.println(MCP.channels()); // Prints # of DAC channels (1)
  Serial.print("MAXVALUE:\t"); Serial.println(MCP.maxValue()); // Prints max value (4095 for a 12 bit DAC)
  Serial.print("INA219_LIB_VERSION: "); Serial.println(INA219_LIB_VERSION); Serial.println();

  delay(1000);

  INA.setMaxCurrentShunt(1, 0.04); // Configure current sensor with max current and shunt resistance
}

void loop() { 
  
}

void voltage_loop() {
  // DAC only has one channel (1), loop function loops through different values (0-4095)    
  for (int value = 0; value <= 4096; value += 315) {
    MCP.write(value, 1); // Args: value (0-4095), channel (1)
    // Note that 4096 exceeds the max value of 4095 but for the purpose of including 4095 this is fine
    
    delay(2000);

    Serial.print("Analog read @ "); Serial.print(value); Serial.print(": ");
    Serial.print(analog_to_voltage()); Serial.println(" V");

    delay(2000);

  }
}

float analog_to_voltage() {
  float Vout = 3.7 * ((float)analogRead(A1)) / 1023.0;
  // Output voltage formula (1024 = 2^10 for 10 bit DAC, Vref = 3.3 V)
  return Vout;
  // Output voltage is off by a scaling factor of ~1.33
  // https://docs.google.com/spreadsheets/d/1Ryu8QGifAIBskT1q3D5ZtqSPAT-eqhYYwvQuIu554fw/edit?usp=sharing
}

void INA_measurements() {
  Serial.print("Bus voltage in mV: "); Serial.println(INA.getBusVoltage_mV());
  Serial.print("Shunt voltage in mV: "); Serial.println(INA.getShuntVoltage_mV());
  Serial.print("Current in mA: "); Serial.println(INA.getCurrent_mA());
}