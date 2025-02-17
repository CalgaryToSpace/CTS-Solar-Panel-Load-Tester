#include "Wire.h"
#include "INA219.h"
#include "stm32f1xx_hal.h"

INA219 current_sensor(0x40);

uint16_t read_alt_current_sense_value() {
    return analogRead(PA0);  //Reading from pin A0(can change if required)
}

int32_t read_solar_panel_voltage_with_on_chip_adc_mV() {
    const float v_sense = 3.3 * ((float)analogRead(PA1)) / 4095.0;
    const float v_panel = v_sense * (12.0 / 2.0);
    return v_panel * 1000.0;
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    if (current_sensor.begin()) {
        Serial.println("INA219 Sensor Connected");
    } else {
        Serial.println("INA219 sensor not connected.");
    }

    current_sensor.setMaxCurrentShunt(1, 0.04);
    delay(500);
    Serial.println("End of setup()...");
}

void loop() {
    for (uint16_t log_num = 0; log_num < 10; log_num++) {
        char json_buffer[400];

        const int32_t adc_panel_voltage_mV = read_solar_panel_voltage_with_on_chip_adc_mV();
        const int32_t bus_voltage_mV = current_sensor.getBusVoltage_mV();
        const int32_t current_mA = current_sensor.getCurrent_mA();

        snprintf(json_buffer, sizeof(json_buffer),
            "{\"uptime_ms\":%li,\"bus_voltage_mV\":%li,\"current_mA\":%li,\"adc_panel_voltage_mV\":%li,\"alt_current_sense\":%u}",
            HAL_GetTick(),
            bus_voltage_mV,
            current_mA,
            adc_panel_voltage_mV,
            read_alt_current_sense_value());

        Serial.println(json_buffer);
        HAL_Delay(100);
    }
}
