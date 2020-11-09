#include "CommandLineUtil.h"
#include "Recorder.h"
#include "RecordingEntryManager.h"

void setup()
{
  Serial.begin(115200);

  while(!Serial)
  {
    delay(1);
  }

  Serial.println("Code begin!");
  Serial1.begin(115200);
  
  // put your setup code here, to run once:
  clu_initDebugCommands();

  unsigned long var;
  Serial.print("Size of unsigned long: ");
  Serial.println(sizeof(unsigned long));

  RecordingEntryManager::createInstance();
}

int loopCounter = 0;
void loop()
{
	// put your main code here, to run repeatedly:

	if(Serial.available())
	{
		char nextChar = Serial.read();
		clu_processCharacter(nextChar);
	}

	pollSerialPort();

	loopCounter++;
	delay(1);

	if ((loopCounter % 10000) == 0)
		Serial.println("Loop");
}
