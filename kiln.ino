#include "rht03.h"
#include "tmp36.h"

const int RHT03_DATA_PIN = D6; // RHT03 data pin
const int TMP36_DATA_PIN = A0;

const String KILN_EVENT = "kiln";

RHT03 rht03;
TMP36 tmp36;

void setup() {
	Serial.begin(9600);

	tmp36.begin(TMP36_DATA_PIN);
	rht03.begin(RHT03_DATA_PIN);
}

void loop() {

	int tmp36Result = tmp36.update();
	int rht03Result = rht03.update();

	if(tmp36Result && rht03Result) {
		Serial.println("RHT03 Humidity: " + String(rht03.humidity(), 2) + " %");
		Serial.println("RHT03 Temp (F): " + String(rht03.tempF(), 2) + " deg F");
		Serial.println("RHT03 Temp (C): " + String(rht03.tempC(), 2) + " deg C");

		Serial.println("TMP36 Volts: " + String(tmp36.volts(), 2));
		Serial.println("TMP36 Temp (C): " + String(tmp36.tempC(), 2) + " deg C");
		Serial.println("TMP36 Temp (F): " + String(tmp36.tempF(), 2) + " deg F");

		if(!publishEvent()) {
			Serial.println("Unable to publish temp event!");
		}
	} else {
		// if we fail, wait a little while for things to reset before we try again
		delay(RHT_READ_INTERVAL_MS);
	}

	delay(2000);
}

bool publishEvent() {
	String eventData = String(rht03.humidity(), 1) + "," + String(rht03.tempF(), 1) + "," + String(tmp36.tempF(), 1);
	return Particle.publish(KILN_EVENT, eventData, 60, PRIVATE);
}
