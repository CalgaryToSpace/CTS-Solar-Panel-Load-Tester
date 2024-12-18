#include "SPI.h"
#include "MCP_DAC.h"
#include "INA219.h"

// Chip select pin for MCP4921
const uint8_t PIN_MCP_CS = 10;
const uint8_t PIN_MCP_MOSI = 3;
const uint8_t PIN_MCP_CLK = 13;

// Initialize DAC: MOSI=D3, CLK=D13.
// Important note: If not using the default hardware pins (MOSI=D11, CLK=D13),
// then DO NOT call `SPI.begin()`!
MCP4921 dac(PIN_MCP_MOSI, PIN_MCP_CLK);

// Initialize INA219 sensor at I2C address 0x40.
INA219 current_sensor(0x40);

uint16_t read_alt_current_sense_value()
{
    return analogRead(A0);
}

/// @brief Reads the analog value from the solar panel and converts it to a voltage.
/// @return The voltage of the solar panel, after applying voltage divider logic.
int32_t read_solar_panel_voltage_with_on_chip_adc_mV()
{
    // Voltage divider: 10k on top, 2k on bottom.
    const float v_sense = 3.3 * ((float)analogRead(A1)) / 1023.0;
    const float v_panel = v_sense * (10.0 + 2.0) / 2.0;
    return v_panel * 1000.0;
}

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
    digitalWrite(PIN_MCP_CS, HIGH); // Disable chip select.

    pinMode(PIN_MCP_CLK, OUTPUT);
    pinMode(PIN_MCP_MOSI, OUTPUT);

    dac.begin(PIN_MCP_CS); // Set chip select pin.
    // Note: DO NOT call SPI.begin() if using software SPI (like we are).
    Serial.println("MCP_DAC Initialized");

    Wire.begin(); // Begin I2C communication (for INA219 sensor).
    if (current_sensor.begin())
    {
        Serial.println("INA219 Sensor Connected");
    }
    else
    {
        Serial.println("INA219 sensor not connected.");
    }

    Serial.print("MCP_DAC_LIB_VERSION: ");
    Serial.println(MCP_DAC_LIB_VERSION);
    Serial.println();
    Serial.print("CHANNELS: ");
    Serial.println(dac.channels()); // Prints # of DAC channels (1)
    Serial.print("MAXVALUE: ");
    Serial.println(dac.maxValue()); // Prints max value (4095 for a 12 bit DAC)
    Serial.print("INA219_LIB_VERSION: ");
    Serial.println(INA219_LIB_VERSION);
    Serial.println();

    delay(1000);

    // Configure current sensor with max current and shunt resistance.
    current_sensor.setMaxCurrentShunt(1, 0.04);

    delay(500); // Wait, in case INA current_sensor is calibrating.

    Serial.println("End of setup()...");
}


void loop()
{
    for (uint16_t dac_value = 0; dac_value < 4096; dac_value += 50)
    {
        set_dac_value(dac_value);

        for (uint16_t log_num = 0; log_num < 10; log_num++)
        {
            char json_buffer[400];

            const int32_t adc_panel_voltage_mV = read_solar_panel_voltage_with_on_chip_adc_mV();
            const int16_t alt_current_sense_value = read_alt_current_sense_value();
            const int32_t bus_voltage_mV = current_sensor.getBusVoltage_mV();
            const int32_t current_mA = current_sensor.getCurrent_mA();

            snprintf(
                json_buffer, sizeof(json_buffer),
                "{"
                "\"uptime_ms\":%li,"
                "\"bus_voltage_mV\":%li,"
                "\"current_mA\":%li,"
                "\"dac_value\":%" PRIi16 ","
                "\"adc_panel_voltage_mV\":%li,"
                "\"alt_current_sense\":%u"
                "}",
                millis(),
                bus_voltage_mV,
                current_mA,
                dac_value,
                adc_panel_voltage_mV,
                alt_current_sense_value);

            Serial.println(json_buffer);

            delay(100);
        }
    }
}
