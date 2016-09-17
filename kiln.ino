#include "rht03.h"
#include "tmp36.h"
#include "thing_speak.h"
#include "twitter_speak.h"
#include "private_data.h"
#include "quote_service.h"

const int RHT03_DATA_PIN = D6;
const int TMP36_1_DATA_PIN = A0;
const int TMP36_2_DATA_PIN = A1;
const int POWER_PIN = D0;

enum ThingSpeakFields {
	THING_SPEAK_RHT03_HUMIDITY_FIELD = 1,
	THING_SPEAK_RHT03_TEMPERATURE_FIELD,
	THING_SPEAK_TMP36_1_TEMPERATURE_FIELD,
	THING_SPEAK_TMP36_2_TEMPERATURE_FIELD
};

const unsigned short THING_SPEAK_SUCCESS = 200;

const int MEASUREMENT_INTERVAL = 60 * 15; // 15 minutes
const int FAILURE_INTERVAL = 20000;

const int WAKEUP_DELAY = 2000;
const int PRE_SLEEP_DELAY = 500;

RHT03 rht03;
TMP36 tmp36_1;
TMP36 tmp36_2;

ThingSpeakClass thingSpeak;
TCPClient client;

TwitterSpeak twitterSpeak;

QuoteService quoteService;

void setup() {
	Serial.begin(9600);

	thingSpeak.begin(client);
	twitterSpeak.begin(THING_TWEET_API_KEY);

	tmp36_1.begin(TMP36_1_DATA_PIN);
	tmp36_2.begin(TMP36_2_DATA_PIN);
	rht03.begin(RHT03_DATA_PIN);
}

void loop() {

	wakeUp();

	int tmp36_1_Result = tmp36_1.update();
	int tmp36_2_Result = tmp36_2.update();
	int rht03Result = rht03.update();

	if(tmp36_1_Result && tmp36_2_Result && rht03Result && voltageDividerResult) {
		Serial.println("RHT03 Humidity: " + String(rht03.humidity(), 2) + " %");
		Serial.println("RHT03 Temp (F): " + String(rht03.tempF(), 2) + " deg F");
		Serial.println("RHT03 Temp (C): " + String(rht03.tempC(), 2) + " deg C");

		Serial.println("TMP36(1) Volts: " + String(tmp36_1.volts(), 2));
		Serial.println("TMP36(1) Temp (C): " + String(tmp36_1.tempC(), 2) + " deg C");
		Serial.println("TMP36(1) Temp (F): " + String(tmp36_1.tempF(), 2) + " deg F");

		Serial.println("TMP36(2) Volts: " + String(tmp36_2.volts(), 2));
		Serial.println("TMP36(2) Temp (C): " + String(tmp36_2.tempC(), 2) + " deg C");
		Serial.println("TMP36(2) Temp (F): " + String(tmp36_2.tempF(), 2) + " deg F");

		if(!publishEvent()) {
			Serial.println("Unable to publish temp event!");
		}

		// go to sleep for a while to save battery life.  this will reset the
		// entire program so we start up from the beginning
		delay(PRE_SLEEP_DELAY);
		/*sleep(MEASUREMENT_INTERVAL);*/
	} else {
		Serial.println("Oh noes!  Couldn't read from our sensors!");
	}

	// we'll only get here if something went wrong.  wait a little while before
	// we try again
	delay(FAILURE_INTERVAL);
}

void wakeUp() {
	pinMode(POWER_PIN, OUTPUT);
	digitalWrite(POWER_PIN, HIGH);

	delay(WAKEUP_DELAY);

	randomSeed(millis());
}

void sleep(int seconds) {
	digitalWrite(POWER_PIN, LOW);
	digitalWrite(POWER_PIN, LOW);
	//System.sleep(SLEEP_MODE_DEEP, seconds);
}

bool publishEvent() {
	int thingSpeakResult;
	bool twitterSpeakResult;
	String quote = "";

	thingSpeak.setField(THING_SPEAK_RHT03_HUMIDITY_FIELD, rht03.humidity());
	thingSpeak.setField(THING_SPEAK_RHT03_TEMPERATURE_FIELD, rht03.tempF());
	thingSpeak.setField(THING_SPEAK_TMP36_1_TEMPERATURE_FIELD, tmp36_1.tempF());
	thingSpeak.setField(THING_SPEAK_TMP36_2_TEMPERATURE_FIELD, tmp36_2.tempF());

	thingSpeakResult = thingSpeak.writeFields(THING_SPEAK_CHANNEL_ID, THING_SPEAK_API_KEY);
	if(thingSpeakResult != THING_SPEAK_SUCCESS) {
		Serial.println("Failed writing to ThingSpeak!");
		return false;
	}

	if(rht03.tempF() < 60) {
		quote = quoteService.getQuote("cold") + "  ";
	} else if(rht03.tempF() > 80 && rht03.tempF() < 100) {
		quote = quoteService.getQuote("warm") + "  ";
	} else if(rht03.tempF() > 100) {
		quote = quoteService.getQuote("hot") + "  ";
	}

	String tweet = quote + "It's " + String(rht03.tempF(), 1) + "F in the kiln right now and " + String(rht03.humidity(), 1) + "% humidity";
	Serial.println(tweet);

	twitterSpeakResult = twitterSpeak.tweet(tweet);
	if(!twitterSpeakResult) {
		Serial.println("Failed writing to Twitter!");
		return false;
	}

	return true;
}
