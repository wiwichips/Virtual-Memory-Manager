/**
	Student Name			Will Pringle
	Student ID #			1056636
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// incldes
#include "dataStructureTLB.h"
#include "dataStructurePageTable.h"

// MACROS
#define BACKING_STORE "BACKING_STORE.bin"
#define SIZE 256

// function protos
void virtualMemoryManager(char* filename);
int getPageDetails(int logicalAddress, int pageSize, int* pageNum, int* offset);
char getValueFromStore(char* filename, int address);
int getFrameNumber(int pageNum);

// user interface functions
char* parseUserData(int argc, char** argv);
void printUsage(char* programName);

// helper functions
int* createFileArray(char* filename, int* numNums);
void init(int TLBsize, int pageTableSize);
void cleanUp();
int logBaseTwo(int x, int ret);
