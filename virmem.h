/**
	Student Name			Will Pringle
	Student ID #			1056636
**/
// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructureTLB.h"
#include "dataStructurePageTable.h"

// MACROS
#define BACKING_STORE "BACKING_STORE.bin"
#define SIZE 256

/// functions
// main functionality protos
/** Central virtual memory manager manager. Calls other parts of the program.
 *@pre TLB and page table have been initialized
 *@post main functionality has finished
 *@return none
 *@param filename - filename for the file to read in the addresses
**/
void virtualMemoryManager(char* filename);

/** gets the page number and page offset from a logical address
 *@pre TLB and page table have been initialized
 *@post nothing is changed but the values page number and offset point to
 *@return page number returend as int, and page num and page offset pointer chan
 *@param logicalAddress - the virtual address
 *@param pageSize - the size of the page in bytes
 *@param pageNum - pointer to the page number so it can be changed
 *@param offset - pointer to the page offset so it can be changed
**/
int getPageDetails(int logicalAddress, int pageSize, int* pageNum, int* offset);

/** gets the value from the backing store
 *@pre backing store file exists, filename is valid, address is valid
 *@post nothing is changed
 *@return byte value from the backing store at the given address, -1 if bad
 *@param filename - the filename for the backing store
 *@param address - the virtual address
**/
char getValueFromStore(char* filename, int address);

/** get the frame number given a page number
 *@pre TLB and Page table exist
 *@post Page table and tlb are updated I think
 *@return the frame number
 *@param pageNum - the frame number that is associated with the page num
**/
int getFrameNumber(int pageNum);

// user interface functions
/** parses the user that the file read in.
 *@pre 
 *@post nothing has changed
 *@return the filename of the addresses text file
 *@param argc - number of arguments
 *@param argv - array of string arguments
**/
char* parseUserData(int argc, char** argv);

/** print the usage for the user if they enter bad information in
 *@pre user has entered incorrect information
 *@post the program is gracefully exited
 *@return none
 *@param programName - name of the program to print during the usage
**/
void printUsage(char* programName);

// helper functions
/** creates an array of int addresess from the addresses text file
 *@pre the filename is not null and num nums it not null
 *@post numNums is updated to point to the number of values in the array
 *@return an alloc'd array of addresses
 *@param filename - the filename to the file of the addresses
 *@param numNums - the number of elements in the array to be passed by reference
**/
int* createFileArray(char* filename, int* numNums);

/** initializes the memory for the TLB and the page table
 *@pre TLB and page table are not already initialized.
 *@post the tlb and page tabled are alloc'd
 *@return none
 *@param TLBsize - the size of the tlb table, by default 16
 *@param pageTableSize - the size of the page table, by default 256
**/
void init(int TLBsize, int pageTableSize);

/** frees the data structure memory for the tlb and the page table
 *@pre the tlb and page table are alloc'd
 *@post the tlb and page table are freed
 *@return none
 *@param none
**/
void cleanUp();

/** function that gives the log base 2 of a number
 *@pre ret is 0
 *@post nothing changed
 *@return the log base 2 of a number x
 *@param x - the power of 2 ^ an unknown exponent
 *@param ret - the number of times the program has recursed, starts at 0
**/
int logBaseTwo(int x, int ret);
