# kiln-photon
Photon library for managing sensors in my solar kiln

# Usage

RHT03 Wiring:
1. Pin 1 goes to a digital pin (D0-D6) for input power
2. Pin 2 goes to a digital input pin (D0-D6)
3. Pin 3 goes nowhere
4. Pin 4 goes to GND

TMP36 Wiring:
1. Pin 1 goes to the same digital pin that was selected for RHT03
2. Pin 2 goes to an analog input pin (A0-A5)
3. Pin 3 goes to GND

The second TMP36 should be wired up the same way, but using a different analog input pin

For the sensor setup:

1. Change the RHT03_DATA_PIN value in kiln.ino to the digital pin you connected it to in the previous set of steps
2. Do the same for the TMP36_1_DATA_PIN and TMP36_2_DATA_PIN value
3. Set POWER_PIN to the digital pin that was selected for input power for the RHT03 and TMP36 sensors
4. Flash your photon!

For the ThingSpeak/ThingTweet integrations:

1. Create a ThingSpeak account
2. Setup a ThingTweet account and connect it to Twitter
3. Add your ThingSpeak channel id and API keys for both ThingSpeak and ThingTweet to private_data.h using example_private_data.h as a reference
4. Flash your photon!
