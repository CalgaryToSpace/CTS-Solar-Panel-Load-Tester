#include <Arduino.h>
#include <INA219.h>

#include <Wire.h>

// R100 shunt resistor = 0.1 ohms
INA219 ina_dev(0x40);

// R2R LSB on D11
// R2R MSB on D3 (9 bits) [set to 0 to keep low always, and make it an 8 bit DAC]

void set_r2r_dac_value(uint8_t value) {
  	digitalWrite(3, LOW); // always low

	// bit_place = 0 is the LSB (rightmost)
	for (uint8_t bit_place = 0; bit_place < 8; bit_place++) {
		uint8_t bit = (value >> bit_place) & 0x01;
		uint8_t pin = 11 - bit_place;
		digitalWrite(pin, bit);
	}
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT); //led
	
	for (uint8_t pin = 3; pin <= 11; pin++) {
		pinMode(pin, OUTPUT);
	}

	set_r2r_dac_value(0); // 0 means off, and thus no current flowing
	
	Serial.begin(115200); // baud rate
	Serial.println("Serial started.");

	// start I2C
	Wire.begin();

	// start INA219
	ina_dev.begin();
	ina_dev.setMaxCurrentShunt(1, 0.1); // 1A max, 0.1 ohm shunt resistor

	Serial.print("INA219 Shunt Resistor (ohms): ");
	Serial.println(ina_dev.getShunt());


	Serial.println("setup() complete");
}

void debug_cycle_through_r2r_values() {
	Serial.println("\nStarting debug_cycle_through_r2r_values()\n");

	// set the DAC value to each value
	for (uint8_t value = 0; value <= 255; value++) {
		set_r2r_dac_value(value);
		delay(50); // 50*250ms = 12.5 seconds
		Serial.print(value);
		Serial.print(" ");
	}
	Serial.println("\nDone debug_cycle_through_r2r_values()\n");
}

void test_current_at_each_r2r_value(uint8_t start_r2r_value, uint8_t max_r2r_value, uint8_t increment_amount) {
	Serial.println("\nStarting test_current_at_each_r2r_value()\n");

	// print the CSV header
	Serial.println("r2r_value,bus_voltage_V,current_mA,shunt_voltage_mV,shunt_adc");

	// set the DAC value to each value
	for (uint16_t value = start_r2r_value; value <= max_r2r_value; value += increment_amount) {
		set_r2r_dac_value(value);
		delay(50); // wait to stabilize

		// print information
		for (uint16_t i = 0; i < 10; i++) {
			float bus_voltage_V = ina_dev.getBusVoltage();
			float current_mA = ina_dev.getCurrent_mA();
			float shunt_voltage_mV = ina_dev.getShuntVoltage_mV();
			uint8_t shunt_adc = ina_dev.getShuntADC();
			Serial.print(value);
			Serial.print(",");
			Serial.print(bus_voltage_V, 3);
			Serial.print(",");
			Serial.print(current_mA, 2);
			Serial.print(",");
			Serial.print(shunt_voltage_mV, 4);
			Serial.print(",");
			Serial.print(shunt_adc);
			Serial.println();
			delay(100); // TODO: change to 100 probably
		}
	}
	Serial.println("\nDone test_current_at_each_r2r_value()\n");
}


void i2c_scanner() {
	byte error, address;
	int nDevices;

	Serial.println("Scanning...");

	nDevices = 0;
	for(address = 1; address < 127; address++ )
	{
	// The i2c_scanner uses the return value of
	// the Write.endTransmission to see if
	// a device did acknowledge to the address.
	Wire.beginTransmission(address);
	error = Wire.endTransmission();

	if (error == 0)
	{
		Serial.print("I2C device found at address 0x");
		if (address<16)
		Serial.print("0");
		Serial.print(address,HEX);
		Serial.println("  !");

		nDevices++;
	}
	else if (error==4)
	{
		Serial.print("Unknown error at address 0x");
		if (address<16)
		Serial.print("0");
		Serial.println(address,HEX);
	}    
	}
	if (nDevices == 0)
	Serial.println("No I2C devices found\n");
	else
	Serial.println("done\n");

	delay(5000);           // wait 5 seconds for next scan
}


void loop() {
	const uint8_t start_r2r_value = 200;
	const uint8_t max_r2r_value = 250;
	const uint8_t increment_amount = 1;
	test_current_at_each_r2r_value(start_r2r_value, max_r2r_value, increment_amount);

	// i2c_scanner();
}
