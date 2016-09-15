#include "application.h"

class VoltageDivider
{
public:
    VoltageDivider() {}

    void begin(int dataPin, float inputVoltage, int r1Resistance, int r2Resistance) {
      _dataPin = dataPin;
      _inputVoltage = inputVoltage;
      _r1Resistance = (float)r1Resistance;
      _r2Resistance = (float)r2Resistance;
    }

    int update() {
      _voltageUnits = analogRead(_dataPin);
      return 1;
    }

    float volts() {
      return (_voltageUnits * (_inputVoltage / 4095)) / (_r2Resistance / (_r1Resistance + _r2Resistance));
    }

private:
    int _dataPin;
    float _inputVoltage;
    float _r1Resistance;
    float _r2Resistance;
    float _voltageUnits;
};
