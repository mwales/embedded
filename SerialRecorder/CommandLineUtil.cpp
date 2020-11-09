#include "CommandLineUtil.h"
#include "Arduino.h"
#include <stdint.h>
#include <string.h>
#include "Recorder.h"
#include "MemoryFree.h"

// Add handler commands to the list, and implementation functions

void memFreeHandler()
{
	Serial.println("Handler 1 called");
	int bytesFree = freeMemory();

	Serial.print("Num bytes free: ");
	Serial.println(bytesFree);
}

void helpHandler();

typedef struct
{ 
	void (*commandHandler)();
	char* commandName;
	char* helpText;
} CmdLineEntry;



// I don't want to waste RAM, so I don't have a dynamic registration system
// Just handjam everything into this list
CmdLineEntry entryList[] = {
	{ memFreeHandler,  "memfree", "Gets RAM bytes RAM free" },
	{ startRecording,  "start",   "Starts recording data from serial" },
	{ statusRecording, "status",  "Gets status of data recording" },
	{ stopRecording,   "stop",    "Stops recording data from serial" },
	{ playRecording,   "play",    "Starts recording playback" },
	{ dumpRecording,   "dump",    "Dumps recording so we can import later" },
	{ importRecording, "import",  "Imports a recording from control serial" },
	{ printCurrentTick,"tick",    "Prints current CPU tick" },
	{ testFunction,    "test",    "Runs internal tests" },
	{ resetBoard,      "reset",   "Reset board" },
	{ hexDump,         "hex",     "Hex Dump" },
	{ helpHandler,     "help",    "lists commands"},
};

// ********************************************************************************
// Everything below this point you should be able to leave as-is
// ********************************************************************************

// cmdLineBuffer length will be length of longest command + 2
char* cmdLineBuffer;
uint8_t cmdLineBufferLen;
uint8_t cmdLineBufferPos;
uint8_t numCmds;

void helpHandler()
{
	uint8_t firstFieldLen = cmdLineBufferLen + 3;
  
	for(int i = 0; i < numCmds; i++)
	{
		// Print the command name
		Serial.print(entryList[i].commandName);

		// Print some spaces / alignment
		uint8_t spaces = firstFieldLen - strlen(entryList[i].commandName);
		while(spaces)
		{
			Serial.print(' ');
			spaces--; 
		}

		// Print command help
		Serial.println(entryList[i].helpText);
	}
}


void clu_initDebugCommands()
{
	numCmds = sizeof(entryList) / sizeof(CmdLineEntry);

	for(uint8_t i = 0; i < numCmds; i++)
	{
		if (i == 0)
		{
			cmdLineBufferLen = strlen(entryList[i].commandName);
		}
		else
		{
			if (strlen(entryList[i].commandName) > cmdLineBufferLen)
			{
	  			cmdLineBufferLen = strlen(entryList[i].commandName);
			}
		}
	}

	Serial.print("Number of debug commands: ");
	Serial.println(numCmds);

	Serial.print("Length of longest command: ");
	Serial.println(cmdLineBufferLen);

	// Add an extra byte for null and /n at then end
	cmdLineBufferLen += 2;

	cmdLineBuffer = (char*) malloc(cmdLineBufferLen);
	memset(cmdLineBuffer, 0, cmdLineBufferLen);
	cmdLineBufferPos = 0;

	helpHandler();
  
}

void processCompleteCommand()
{
	// Serial.print("We received the following command: ");
	// Serial.println(cmdLineBuffer);

	for(uint8_t i = 0; i < numCmds; i++)
	{
		if (strcmp(cmdLineBuffer, entryList[i].commandName) == 0)
		{
			entryList[i].commandHandler();
			goto clean_buffer_and_return;
		}
	}

	Serial.print("Unknown command: ");
	Serial.println(cmdLineBuffer);

	helpHandler();

clean_buffer_and_return:
	// Clean the buffer and reset pointer
	cmdLineBufferPos = 0;
	memset(cmdLineBuffer, 0, cmdLineBufferLen);
}

void clu_processCharacter(char singleChar)
{
	/*
	Serial.print(cmdLineBufferPos);
	Serial.print(" Processing: ");
	Serial.print((int) singleChar);
	Serial.print(" ");
	Serial.println(singleChar);
	*/

	bool deleteDetected = false;

	if ( (singleChar == 0x08) || (singleChar == 0x7f) )
	{
		// Serial.println("Detected backspace");
		if (cmdLineBufferPos > 0)
		{
			cmdLineBufferPos--;
			cmdLineBuffer[cmdLineBufferPos] = 0;
		}
		return;
	}

	if (singleChar == 0x1B)
	{
		if (Serial.available() && Serial.read() == 0x5b)
		{
			if (Serial.available() && Serial.read() == 0x33)
			{
				if (Serial.available() && Serial.read() == 0x7e)
				{
					// Serial.println("Escape code delete detected");
					deleteDetected = true;
				}
			}
		}
	}

	if (deleteDetected)
	{
		if (cmdLineBufferPos > 0)
		{
			cmdLineBufferPos--;
			cmdLineBuffer[cmdLineBufferPos] = 0;
		}
		return;
	}

	if ( (singleChar == '\n') || (singleChar == '\r') )
	{
		// echo newline
		Serial.println("");

		// See if there is anything in the current command at all
		if (cmdLineBufferPos == 0)
		{
			// Blank command
			return;
		}
		
		processCompleteCommand();
		return;
	}

	// echo character
	Serial.print(singleChar);

	// Add the new character to the end of the string
	cmdLineBuffer[cmdLineBufferPos] = singleChar;
	
	// Are we out of room?
	if (strlen(cmdLineBuffer) == (cmdLineBufferLen - 1) )
	{
		// We have no more room for characters, process what we have
		processCompleteCommand();
		return;
	}

	cmdLineBufferPos++;
}

