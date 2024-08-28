#include "MCP_DAC.h"
#include "SPI.h"

MCP4921 MCP(11, 13); // Args: MOSI=D11, CLK=D13
// MCP4921 MCP;  //  HW SPI, Default: MOSI=D11, CLK=D13

float analog_to_voltage();

void setup() {  
  Serial.begin(115200);
  Serial.println(__FILE__);
  // SPI.begin(); // Use with HW SPI only

  MCP.begin(10); // Set pin 10 to OUTPUT and HIGH
  
  Serial.print("MCP_DAC_LIB_VERSION: "); Serial.println(MCP_DAC_LIB_VERSION); Serial.println();
  Serial.print("CHANNELS:\t"); Serial.println(MCP.channels()); // Prints # of DAC channels (1)
  Serial.print("MAXVALUE:\t"); Serial.println(MCP.maxValue()); // Prints max value (4095 for a 12 bit DAC)
  
  delay(100);
  Serial.println("\nLooping:");
}

void loop() { 
  // DAC only has one channel (1), loop function loops through different values (0-4095)    
  for (int value = 0; value <= 4096; value += 512) {
    MCP.write(value, 1); // Args: value (0-4095), channel (1)
    // Note that 4096 exceeds the max value of 4095 but for the purpose of including 4095 this is fine
    
    if (MCP.write(value, 0) == false) {
      Serial.print("invalid channel");
    }
    
    delay(2000);

    Serial.print("Analog read @ "); Serial.print(value); Serial.print(": ");
    Serial.print(analog_to_voltage()); Serial.println(" V");

     delay(2000);
  }
}

float analog_to_voltage() {
  float Vout = 3.3 * ((float)analogRead(A1)) / 1024.0; 
  // Output voltage formula (1024 = 2^10 for 10 bit DAC, Vref = 3.3 V)
  return Vout;
  // Output voltage is off by a scaling factor of ~1.33
  // https://docs.google.com/spreadsheets/d/1Ryu8QGifAIBskT1q3D5ZtqSPAT-eqhYYwvQuIu554fw/edit?usp=sharing
}