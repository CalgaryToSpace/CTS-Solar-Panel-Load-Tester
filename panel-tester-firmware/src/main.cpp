#include "MCP_DAC.h"
#include "SPI.h"

MCP4921 MCP(11, 13); // Args: MOSI=D11, CLK=D13
// MCP4921 MCP;  //  HW SPI, Default: MOSI=D11, CLK=D13

volatile int x;
uint32_t start, stop;

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
   // MCP.write(value, 20); // Args: value (0-4095), channel (1)
    
    if (MCP.write(value, 0) == false) {
      Serial.print("invalid channel");
    }

   // Serial.print("Analog read @ "); Serial.print(value); Serial.print(": "); 
   // Serial.print(analog_to_voltage()); Serial.println(" V");
   Serial.print("Set");
  delay(2000);
  }

  
}

float analog_to_voltage() {
  return 3.3 * analogRead(A1) / 1024; // Converts analog reading to voltage measurement
}