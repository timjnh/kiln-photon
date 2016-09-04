class TMP36
{
public:
  TMP36();

  void begin(int dataPin);

  int update();

  float volts();
  float tempC();
  float tempF();

private:

  int _dataPin;

  int _voltageUnits;
};
