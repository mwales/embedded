#include "RecordingEntryManager.h"
#include "MemoryFree.h"

#include "Arduino.h"
#include <string.h>

RecordingEntryManager* RecordingEntryManager::theInstancePtr = nullptr;

RecordingEntryManager* RecordingEntryManager::createInstance()
{
	if (theInstancePtr != nullptr)
	{
		printStrLn("Trying to create the RecordingEntryManager twice!");
		return theInstancePtr;
	}

	theInstancePtr = new RecordingEntryManager();
	return theInstancePtr;
}

RecordingEntryManager* RecordingEntryManager::getInstance()
{
	if (theInstancePtr == nullptr)
	{
		printStrLn("No instance of RecordingEntryManager created yet");
	}

	return theInstancePtr;
}

void RecordingEntryManager::deleteInstance()
{
	if (theInstancePtr == nullptr)
	{
		printStrLn("Can't delete non-existing RecordingEntryManager singleton");
		return;
	}

	delete theInstancePtr;
	theInstancePtr = nullptr;
}

RecordingEntryManager::RecordingEntryManager():
	theBuffer(0),
	theCurrentIndex(0xffffffff),
	theBufferLen(0),
	theCurrentEntryHeader(0)
{
	// Intentially empty

}

bool RecordingEntryManager::initMemoryWithReserve(int bytesToReserveOnHeap)
{
	int freeMem = freeMemory();

	if (bytesToReserveOnHeap > freeMem)
	{
		printStr("REM: Not enough free memory: ");
		printInt(freeMem);
		printStrLn(" bytes");
		return false;
	}

	return initMemoryOfSize(freeMem - bytesToReserveOnHeap);
}

bool RecordingEntryManager::initMemoryOfSize(int bytesToReserve)
{
	if (bytesToReserve < sizeof(RecEntry))
	{
		printStrLn("REM: Not enough bytes reserved for a single entry");
		return false;
	}
	
	theBufferLen = bytesToReserve;

	printStr("The Buffer is created with len = ");
	printInt(theBufferLen);
	printStrLn("");
	
	theBuffer = (char*) malloc(theBufferLen);
	if (theBuffer == 0)
	{
		printStr("REM: malloc failed for ");
		printInt(theBufferLen);
		printStrLn(" bytes");
		return false;
	}
	memset(theBuffer, 0, theBufferLen);

	theCurrentIndex = 0;

	// Setup the first entry
	theCurrentEntryHeader = (RecEntry*) theBuffer;
}

bool RecordingEntryManager::startRecorder()
{
	if (theCurrentIndex == 0xffffffff)
	{
		printStrLn("REM: initMemory before startRecorder()");
		return false;
	}

	theStartTick = millis();
	theCurrentEntryHeader->theState = STATE_FIRST;
	theCurrentEntryHeader->theState |= STATE_CLOSED;
	theCurrentEntryHeader->theState |= STATE_LAST;
	theCurrentEntryHeader->theLen = 0;
	return true;
}

bool RecordingEntryManager::addCharacter(char x)
{
	if (theCurrentIndex == 0xffffffff)
	{
		printStrLn("REM: startRecording before addCharacter");
		return false;
	}

	if (theCurrentEntryHeader->theState & STATE_CLOSED)
	{
		printStrLn("Entry currently closed, but we have to add char");
		if (theCurrentEntryHeader->theLen)
		{
			printStrLn("REM: Entry is closed + has non-zero len.  Next!");
			// This entry had data in it already, goto the next entry
			if (!gotoNextEntry())
			{
				printStrLn("REM: No more room to make new entries");
				return false;
			}
		}

		// We need to open the record first
		printStrLn("REM: Making new entry!");
		theCurrentEntryHeader->theState &= ~STATE_CLOSED;
		theCurrentEntryHeader->theState |= STATE_OPEN;
		theCurrentEntryHeader->theTick = millis() - theStartTick;
		theCurrentIndex += sizeof(RecEntry);
	}
	else
	{
		printStrLn("Entry already open");
	}

	// Do we have room to add another character?
	if (theCurrentIndex < theBufferLen - 1)
	{
		theBuffer[theCurrentIndex++] = x;
		theCurrentEntryHeader->theLen++;
		return true;
	}
	else
	{
		printStrLn("REM: No more room to add a single character");
		return false;
	}
}

bool RecordingEntryManager::gotoNextEntry()
{
	if (getLengthOfCurrentEntry() == 0)
	{
		return false;
	}
	
	// Goto the nearest 4 byte 
	uint32_t padding = 4 - theCurrentIndex & 0x3;
	if (padding == 4)
	{
		padding = 0;
	}

	if ( (theCurrentIndex + padding + sizeof(RecEntry)) < theBufferLen)
	{
		theCurrentIndex += padding;
		theCurrentEntryHeader = (RecEntry*) &theBuffer[theCurrentIndex];
		theCurrentIndex += sizeof(RecEntry);
		return true;
	}
	else
	{
		printStrLn("REM: Not enough room for next RecEntry");
		return false;
	}
	
}

bool RecordingEntryManager::closeAndAdvanceEntry()
{
	printStr("closeAndAdvanceEntry: ");
	theCurrentEntryHeader->theState |= STATE_CLOSED;
	theCurrentEntryHeader->theState &= ~STATE_OPEN;

	// Save the previous
	RecEntry* prevEntry = theCurrentEntryHeader;

	while( (theCurrentIndex % 4) != 0)
	{
		theCurrentIndex++;
		printStr("Adding padding byte. ");
	}
	printStrLn("");

	RecEntry* nextEntry = (RecEntry*) &theBuffer[theCurrentIndex];

	theCurrentIndex += sizeof(RecEntry);

	// Make sure the RecEntry doesn't extend past the end of the buffer
	if (theCurrentIndex > theBufferLen)
	{
		// We are completely out of room for any more entries
		theCurrentEntryHeader = prevEntry;

		theCurrentIndex = (int) theCurrentEntryHeader - (int) theBuffer + theCurrentEntryHeader->theLen;

		// If it is not already marked as last, do so
		theCurrentEntryHeader->theState |= STATE_LAST;

		printStrLn("No more room for any more records");

		return false;
	}
	else
	{
		theCurrentEntryHeader = nextEntry;
		theCurrentEntryHeader->theState |= STATE_CLOSED;
		theCurrentEntryHeader->theState |= STATE_LAST;
		return true;
	}
	
}

void RecordingEntryManager::printEntry(bool debug)
{
	/*uint16_t* len = (uint16_t*) theBuffer;

	for(uint16_t i = 0; i < *len; i++)
	{
		Serial.print( (char) theBuffer[i+sizeof(uint16_t)];
	}
	*/
}

uint16_t RecordingEntryManager::getLengthOfCurrentEntry()
{
	return theCurrentEntryHeader->theLen;
}


void RecordingEntryManager::printInt(int number)
{
	Serial.print(number);
}

void RecordingEntryManager::printStr(char const * const str)
{
	Serial.print(str);
}

void RecordingEntryManager::printStrLn(char const * const str)
{
	Serial.println(str);
}

void RecordingEntryManager::printChar(char x)
{
	Serial.print(x);
}

void RecordingEntryManager::printStatus()
{
	printStr("theBuffer =");
	printHexU32( (uint32_t) theBuffer);
	printStrLn("");

	printStr("theCurrentIndex = ");
	printInt(theCurrentIndex);
	printStrLn("");

	printStr("theBufferLen = ");
	printInt(theBufferLen);
	printStrLn("");

	printStr("theStartTick = ");
	printInt(theStartTick);
	printStrLn("");

	printStr("theCurrentEntryHeader (offset) = ");
	printHexU32( (uint32_t) theCurrentEntryHeader);
	printStrLn("");

	if (theBuffer == nullptr)
	{
		printStrLn("No data to print");
		return;
	}


	printStrLn("*** Start walking data ***");
	printStrLn("ENTRY\tTICK\tLEN\tDATA");

	// Walk through the entries
	uint32_t walkIndex = 0;
	uint32_t entryNum = 0;
	while(walkIndex < theCurrentIndex)
	{
		printStr("walkIndex = ");
		printInt(walkIndex);
		printStrLn("");

		// Find the next entry
		if (walkIndex % 4 != 0)
		{
			walkIndex++;
			continue;
		}

		RecEntry* re = (RecEntry*) &theBuffer[walkIndex];

		walkIndex += sizeof(RecEntry);

		if (walkIndex >= theCurrentIndex)
		{
			printStrLn("No more room for RecEntry in data buffer");
			break;
		}

		// Print out the contents of the entry header
		printInt(entryNum);
		printStr("\t");
		
		printInt(re->theTick);
		printStr("\t");

		printInt(re->theLen);
		printStr("\t");

		if (re->theState & STATE_FIRST)
			printStr("F");

		if (re->theState & STATE_LAST)
			printStr("L");

		if (re->theState & STATE_OPEN)
			printStr("O");

		if (re->theState & STATE_CLOSED)
			printStr("C");
		printStr("\t");

		// Print out the contents of the entry body
		for(int i = 0; i <= re->theLen; i++)
		{
			printChar(theBuffer[walkIndex]);

			walkIndex++;

			if (walkIndex >= theCurrentIndex)
			{
				printStrLn("\nERR: walk index finished while printing entry");
				break;
			}
		}

		printStrLn("");
		entryNum++;

	}

	printStrLn("*** Done walking data ***");

}

void RecordingEntryManager::printHexByte(char x)
{
	char firstChar = (x >> 4) & 0xf;
	char secondChar = x & 0xf;

	if (firstChar >= 10)
	{
		firstChar += 'a' - 10;
	}
	else
	{
		firstChar += '0';
	}

	if (secondChar >= 10)
	{
		secondChar += 'a' - 10;
	}
	else
	{
		secondChar += '0';
	}

	printChar(firstChar);
	printChar(secondChar);
}

void RecordingEntryManager::printHexU16(uint16_t x)
{
	printHexByte( (x >> 8) & 0xff);
	printHexByte( x & 0xff);
}

void RecordingEntryManager::printHexU32(uint32_t x)
{
	printHexByte( (x >> 24) & 0xff);
	printHexByte( (x >> 16) & 0xff);
	printHexByte( (x >> 8) & 0xff);
	printHexByte( x & 0xff);
}

void RecordingEntryManager::hexDump(int numBytes)
{

	if (numBytes > theBufferLen)
	{
		printStr("Hex bytes limited to ");
		printInt(theBufferLen);
		printStrLn(" bytes.");
		numBytes = theBufferLen;
	}

	for(int i = 0; i < numBytes; i++)
	{
		printHexByte(theBuffer[i]);
		printChar(' ');

		if ( (i != 0) && ((i % 16) == 15) )
		{
			printStrLn(" ");
		}
	}

	printStrLn("");

}



