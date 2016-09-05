#include "rht03.h"
#include "tmp36.h"
#include "thing_speak.h"
#include "twitter_speak.h"
#include "private_data.h"

const int RHT03_DATA_PIN = D6; // RHT03 data pin
const int TMP36_DATA_PIN = A0;

enum ThingSpeakFields {
	THING_SPEAK_RHT03_HUMIDITY_FIELD = 1,
	THING_SPEAK_RHT03_TEMPERATURE_FIELD,
	THING_SPEAK_TMP36_TEMPERATURE_FIELD
};

const unsigned short THING_SPEAK_SUCCESS = 200;

const int MEASUREMENT_INTERVAL = 60;
const int FAILURE_INTERVAL = 20000;

const int WAKEUP_DELAY = 2000;
const int PRE_SLEEP_DELAY = 500;

RHT03 rht03;
TMP36 tmp36;

ThingSpeakClass thingSpeak;
TCPClient client;

TwitterSpeak twitterSpeak;

void setup() {
	Serial.begin(9600);

	thingSpeak.begin(client);
	twitterSpeak.begin(THING_TWEET_API_KEY);

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
	int thingSpeakResult;
	bool twitterSpeakResult;

	String tweet = "It's " + String(rht03.tempF(), 1) + "F in the kiln right now and " + String(rht03.humidity(), 1) + "% humidity";

	twitterSpeakResult = twitterSpeak.tweet(tweet);
	if(!twitterSpeakResult) {
		Serial.println("Failed writing to Twitter!");
		return false;
	}

	thingSpeak.setField(THING_SPEAK_RHT03_HUMIDITY_FIELD, rht03.humidity());
	thingSpeak.setField(THING_SPEAK_RHT03_TEMPERATURE_FIELD, rht03.tempF());
	thingSpeak.setField(THING_SPEAK_TMP36_TEMPERATURE_FIELD, tmp36.tempF());

	thingSpeakResult = thingSpeak.writeFields(THING_SPEAK_CHANNEL_ID, THING_SPEAK_API_KEY);
	if(thingSpeakResult != THING_SPEAK_SUCCESS) {
		Serial.println("Failed writing to ThingSpeak!");
		return false;
	}

	return true;
}
