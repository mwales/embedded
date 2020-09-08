#include "CommandLineUtil.h"

void setup()
{
  Serial.begin(115200);

  while(!Serial)
  {
    delay(1);
  }

  Serial.println("Code begin!");
  
  // put your setup code here, to run once:
  clu_initDebugCommands();


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

	loopCounter++;
	delay(1);

	if ((loopCounter % 10000) == 0)
		Serial.println("Loop");
}
