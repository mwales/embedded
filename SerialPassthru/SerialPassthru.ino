// Passing through serial from GPS module connected to Feather M0 LoRa to our
// serial connection (host beinga Raspberry Pi)

// To compile and flash using Arudino CLI
// arduino-cli compile -b adafruit:samd:adafruit_feather_m0
// arduino-cli upload -b adafruit:samd:adafruit_feather_m0 -p /dev/ttyACM0


void setup()
{
	Serial.begin(9600);
	Serial1.begin(9600);

	while(!Serial)
	{
		delay(1);
	}

	Serial.println("Serial passthru @ 9600");

	Serial1.begin(9600);
	
}

int counter = 0;


void loop()
{
	if (Serial.available())
	{
		Serial1.write(Serial.read());
	}

	if (Serial1.available())
	{
		Serial.write(Serial1.read());
	}
}

