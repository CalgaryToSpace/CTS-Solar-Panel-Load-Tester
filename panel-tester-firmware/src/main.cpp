#include "SPI.h"
#include "MCP_DAC.h"

// Chip select pin for MCP4921
const uint8_t PIN_MCP_CS = 10;
const uint8_t PIN_MCP_MOSI = 3;
const uint8_t PIN_MCP_CLK = 13;

// Initialize DAC: MOSI=D3, CLK=D13.
// Important note: If not using the default hardware pins (MOSI=D11, CLK=D13),
// then DO NOT call `SPI.begin()`!
MCP4921 dac(PIN_MCP_MOSI, PIN_MCP_CLK);



/// @brief Sets the DAC value to a new value.
/// @param new_value 12-bit value to set the DAC to (0 to 4095).
void set_dac_value(uint16_t new_value)
{
    if (! dac.write(new_value, 0))
    {
        Serial.println("Failed to write to DAC.");
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Booting...");

    // Init pin modes.
    pinMode(PIN_MCP_CS, OUTPUT);
    digitalWrite(PIN_MCP_CS, HIGH); // Disable chip select. - why are we disabling when we need the clk and data?

    pinMode(PIN_MCP_CLK, OUTPUT);
    pinMode(PIN_MCP_MOSI, OUTPUT);

    dac.begin(PIN_MCP_CS); // Set chip select pin.
    // Note: DO NOT call SPI.begin() if using software SPI (like we are).
    Serial.println("MCP_DAC Initialized");

    
    Serial.print("MCP_DAC_LIB_VERSION: ");
    Serial.println(MCP_DAC_LIB_VERSION);
    Serial.println();
    Serial.print("CHANNELS: ");
    Serial.println(dac.channels()); // Prints # of DAC channels (1)
    Serial.print("MAXVALUE: ");
    Serial.println(dac.maxValue()); // Prints max value (4095 for a 12 bit DAC)

    delay(1000);

    Serial.println("End of setup()...");
}


void loop()
{
    for (uint16_t dac_value = 0; dac_value < 4096; dac_value += 50)
    {
        set_dac_value(dac_value);
        Serial.print("dac: ");
        Serial.println(dac_value);
        delay(1000);
    }
}
