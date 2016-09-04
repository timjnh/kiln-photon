#include "rht03.h"
#include "tmp36.h"

const int RHT03_DATA_PIN = D6; // RHT03 data pin
const int TMP36_DATA_PIN = A0;

const String KILN_EVENT = "kiln";

const int MEASUREMENT_INTERVAL = 60;
const int FAILURE_INTERVAL = 2000;

const int WAKEUP_DELAY = 2000;
const int PRE_SLEEP_DELAY = 500;

RHT03 rht03;
TMP36 tmp36;

void setup() {
	Serial.begin(9600);

	tmp36.begin(TMP36_DATA_PIN);
	rht03.begin(RHT03_DATA_PIN);

	delay(WAKEUP_DELAY);
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
		} else {
			// if we were successful, go to sleep for a while to save battery life.
			// this will reset the entire program so we start up from the beginning
			delay(PRE_SLEEP_DELAY);
			//System.sleep(SLEEP_MODE_DEEP, MEASUREMENT_INTERVAL);
		}
	}

	// we'll only get here if something went wrong.  wait a little while before
	// we try again
	delay(FAILURE_INTERVAL);
}

bool publishEvent() {
	String eventData = String(rht03.humidity(), 1) + "," + String(rht03.tempF(), 1) + "," + String(tmp36.tempF(), 1);
	return Particle.publish(KILN_EVENT, eventData, 60, PRIVATE);
}
