#include "Recorder.h"
#include "Arduino.h"
#include "MemoryFree.h"
#include "RecordingEntryManager.h"


#define RECORDING_PORT Serial1
#define CONTROL_PORT   Serial

void startRecording()
{
	Serial.println("Start recording");
	RecordingEntryManager* rem = RecordingEntryManager::getInstance();

	if (rem == nullptr)
	{
		CONTROL_PORT.println("RecordingEntryManager returned nullptr");
		return;
	}

	rem->initMemoryWithReserve(200);
	rem->startRecorder();	

}

void statusRecording()
{
	Serial.println("Status recording");

	RecordingEntryManager* rem = RecordingEntryManager::getInstance();

	if (rem == nullptr)
	{
		CONTROL_PORT.println("RecordingEntryManager returned nullptr");
		return;
	}

	rem->printStatus();
}

void stopRecording()
{
	Serial.println("Stop recording");
}

void playRecording()
{
	Serial.println("Play recording");
}

void dumpRecording()
{
	Serial.println("Dump recording");
}

void importRecording()
{
	Serial.println("Import recording");
}

void pollSerialPort()
{
	while(RECORDING_PORT.available())
	{
		CONTROL_PORT.print("RX-ed: ");
		char x = Serial1.read();
		CONTROL_PORT.println(x);

		RecordingEntryManager* rem = RecordingEntryManager::getInstance();
		if (rem == nullptr)
		{
			// Recorder not setup yet
			return;
		}

		if( (x == '\n') || (x == '\r') )
		{	
			rem->closeAndAdvanceEntry();
		}
		else
		{
			rem->addCharacter(x);
		}
	}
}

void printCurrentTick()
{
	uint32_t curTick = millis();
	CONTROL_PORT.print("Current tick=");
	CONTROL_PORT.println(curTick);
}

void testFunction()
{
	uint32_t sizeOfRecEntry = sizeof(RecEntry);

	CONTROL_PORT.print("Size of RecEntry (bytes) = ");
	CONTROL_PORT.println(sizeOfRecEntry);
}

void (*resetFunc)(void) = 0;

void resetBoard()
{
	CONTROL_PORT.println("Resetting board...");
	delay(1);
	resetFunc();
}

void hexDump()
{
	RecordingEntryManager* rem = RecordingEntryManager::getInstance();
	rem->hexDump(9999);
}
