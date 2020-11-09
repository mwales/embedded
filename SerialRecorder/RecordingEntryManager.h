#ifndef RECORDING_ENTRY_H
#define RECORDING_ENTRY_H

#include <stdint.h>



#define STATE_FIRST   0x01
#define STATE_LAST    0x02
#define STATE_OPEN    0x04
#define STATE_CLOSED  0x08
#define STATE_UNDEF   0xff


typedef struct RecEntry
{
	uint32_t theTick;
	uint16_t theLen;
	uint8_t theState;
} RecEntry;

class RecordingEntryManager
{
public:

	static RecordingEntryManager* createInstance();
	static RecordingEntryManager* getInstance();
	static void deleteInstance();

	bool initMemoryWithReserve(int bytesToReserveOnHeap);


	bool initMemoryOfSize(int bytesToReserve);
	
	/**
	 * Start the recording (note what tick we are on now)
	 */
	bool startRecorder();

	/**
	 * Adds a character to the current recording entry.  Returns false
	 * if there is no more room
	 */
	bool addCharacter(char x);

	/**
	 * Sets current pointer to the next entry.  Returns false if we are
	 * already at the last entry
	 */
	bool gotoNextEntry();

	/**
	 * Closes the current entry and moves pointer to the next entry
	 */
	bool closeAndAdvanceEntry();

	/**
	 * Sets current pos to the previous entry.  Returns false if we are
	 * already at the first entry
	 */
	bool gotoPrevEntry();

	void printEntry(bool debug);

	uint16_t getLengthOfCurrentEntry();

	void printStatus();

	void hexDump(int numBytes);

protected:

	// Singleton
	RecordingEntryManager();
	~RecordingEntryManager();

	static void printInt(int number);
	static void printStr(char const * const str);
	static void printStrLn(char const * const str);
	static void printChar(char x);
	static void printHexByte(char x);
	static void printHexU16(uint16_t x);
	static void printHexU32(uint32_t x);

	char* theBuffer;

	uint32_t theCurrentIndex;

	uint32_t theBufferLen;

	unsigned long theStartTick;

	RecEntry* theCurrentEntryHeader;

	static RecordingEntryManager* theInstancePtr;
};


#endif
