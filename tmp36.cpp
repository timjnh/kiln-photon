#include "tmp36.h"
#include "application.h"

TMP36::TMP36() {}

void TMP36::begin(int dataPin) {
  _dataPin = dataPin;
}

int TMP36::update() {
	_voltageUnits = analogRead(_dataPin);
  return 1;
}

float TMP36::volts() {
  // analogread will give a value between 0 and 4095.  this maps to a range of
	// voltages from 0 to 3.3v (although in reality it seems closer to 3.15)
  return _voltageUnits * (3.15 / 4095);
}

float TMP36::tempC() {
  // the TMP36 reads 0.1v at ~-40C and 1.7v at 125C (C/v = 125 - -40 / 1.7 - .1)
	return volts() * (165 / 1.6) - 50;
}

float TMP36::tempF() {
  return tempC() * 9.0 / 5.0 + 32.0;
}
