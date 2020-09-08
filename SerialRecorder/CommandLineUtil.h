#ifndef COMMAND_LINE_UTIL_H
#define COMMAND_LINE_UTIL_H

// Call this at beginning of program to init buffers
void clu_initDebugCommands();

// Call with every single character received (will echo all these chars back to serial)
void clu_processCharacter(char singleChar);

#endif
