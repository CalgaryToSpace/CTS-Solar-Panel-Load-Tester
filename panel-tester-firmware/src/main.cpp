#include <Arduino.h>
#include <INA219.h>

INA219 INA(0x40);

void setup() {
	pinMode(LED_BUILTIN, OUTPUT); //led
	pinMode(10, OUTPUT);
	pinMode(8, OUTPUT);
	digitalWrite(8, LOW); // for capacitor
	Serial.begin(115200); //baud rate 
	Serial.println("Setup complete");
}

void loop() {
	digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on 


	//analog write - 100 = 0.5V
	analogWrite(10, 50);

	//print information
	float bus_voltage = INA.getBusVoltage_mV();
	//float shunt_voltage = INA.getShuntVoltage_mV();
	float current = INA.getCurrent_mA();

	//Serial.println("Format: bus voltage (V), current(mA)");
	Serial.print(bus_voltage, 2);
	Serial.print(",");
	Serial.print(current, 2);
	Serial.println(",");

	delay(1000);                      
	digitalWrite(LED_BUILTIN, LOW);   // turn the LED off 
	delay(1000);

	
}
