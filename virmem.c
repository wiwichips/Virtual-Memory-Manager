#include "virmem.h"

int main(int argc, char** argv) {
	
	char* filename = parseUserData(argc, argv);
	
	init(16, SIZE);
	virtualMemoryManager(filename);
	cleanUp();
	return 0;
}

void virtualMemoryManager(char* filename) {
	// initialize variables
	int numNums = 0;
	int* nums = createFileArray(filename, &numNums);
	int pageNum = 0;
	int pageOff = 0;	
	int tmp = 0;
	
	for(int i = 0; i < numNums; i++) {
		getPageDetails(nums[i], SIZE, &pageNum, &pageOff);
		tmp = getFrameNumber(pageNum);
		
		printf("Virtual address: %d Physical address: %d Value: %d", nums[i], tmp 
		+ pageOff, getValueFromStore(BACKING_STORE, nums[i]));
		puts("");
	}

	fprintf(stderr, "Number of Translated Addresses = %d\n", numNums);
	fprintf(stderr, "Page Faults = %d\n", g_pageFaults);
	fprintf(stderr, "Page Fault Rate = %.3f\n", ((float) g_pageFaults) / ((float) numNums));
	fprintf(stderr, "TLB Hits = %d\n", g_hitsTLB);
	fprintf(stderr, "TLB Hit Rate = %.3f\n", ((float) g_hitsTLB) / ((float) numNums));
	// free numbers
	free(nums);
}

int getPageDetails(int logicalAddress, int pageSize, int* pageNum, int* offset) {	
	// check if pointers are NULL or pageSize is invalid
	if(!pageNum || !offset || (pageSize % 2)) { return - 1; }
	
	// convert pageSize to number of bits
	int numBits = logBaseTwo(pageSize, 0);
	
	*pageNum = logicalAddress >> numBits; // truncate first 10 bits
	*offset = logicalAddress - ((logicalAddress >> numBits) << numBits);
	
	return *pageNum;
}

char getValueFromStore(char* filename, int address) {
	char value = 0;
	FILE* fp = NULL;
	
	// attempt to open the file
	fp = fopen(filename, "rb");
	
	fseek(fp, address, SEEK_SET);
	
	fscanf(fp, "%c", &value);
	
	// close file
	fclose(fp);
	return value;
}

int getFrameNumber(int pageNum) {
	int frame = -1;

	/// first check the TLB
	frame = getFrameTLB(pageNum);
	
	if(frame < 0) {
		
	} else {
		g_hitsTLB++;
		return frame;
	}
	
	/// check the page table
	frame = pageToFrame(pageNum);
	
	// add the frame to the page table
	addToTLB(pageNum, frame);
	
	return frame;
}

// helper functions
int* createFileArray(char* filename, int* numNums) {
	int* nums = NULL;
	FILE* fp = NULL;
	int i = 0;
	
	// attempt to open the file
	fp = fopen(filename, "r");
	
	// if the file does not successfully open
	if(!fp) {
		return NULL;
	}
	
	//get the file size
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
	
	// read the file into the nums array
	for(i = 0; ftell(fp) < size; i++) {		
		// allocate memory in the array
		nums = realloc(nums, (i + 1) * sizeof(int));
		
		// put the number in the array
		fscanf(fp, "%d", &nums[i]);	
	}
	
	// set the number of addresses to i
	*numNums = i;
	
	// subtract one if the last line is a new line as not to miss an address
	fseek(fp, -1, SEEK_END);
	char lastCharacter = fgetc(fp);
	if('\n' == lastCharacter) {
		(*numNums)--;
	}
	
	// close file
	fclose(fp);
		
	if(!nums) {
		puts("nums == NULL");
		return NULL;
	}
	
	return nums;
}

int logBaseTwo(int x, int ret) {
	if(x == 0) {
		return --ret;
	}
	
	return logBaseTwo(x/2, ++ret);
}

void init(int TLBsize, int pageTableSize) {
	g_TLB = calloc(TLBsize, sizeof(Map));
	
	g_pageTable= calloc(pageTableSize, sizeof(int));
	
	// initialize all values in page table to -1
	memset(g_pageTable, -1, pageTableSize * sizeof(int));
}

void cleanUp() {
	for(int i = 0; i < g_numEntriesTLB; i++) {
		free(g_TLB[i]);
	}
	free(g_TLB);
	free(g_pageTable);
}

// tlb functions
void addToTLB(int pageNum, int frameNum) {
	
	// first check if the item is already in the TLB
	if(getFrameTLB(pageNum) == frameNum) {
		return; // do not add to the tlb if its already there
	}
	
	// temp variables
	Map* temp1 = 0;
	Map* temp2 = 0;
	
	// allocate space for new value
	Map* newMap = 0;
	newMap = calloc(1, sizeof(Map));
	
	// place the values into the new map
	newMap->key = pageNum;
	newMap->value = frameNum;
	
	// free the last element if the TLB is full
	if(g_numEntriesTLB >= 16) {
		free(g_TLB[15]);
	} else {
		g_numEntriesTLB++;
	}
	
	// set temp1 to the new value
	temp1 = newMap;
	
	// shift everything forward by 1 and add new value at the start
	for(int i = 0; i < g_numEntriesTLB; i++) {
		temp2 = g_TLB[i];
		
		g_TLB[i] = temp1;
		
		temp1 = temp2;
	}
	
}

int getFrameTLB(int pageNum) {
	// loop through each map to determine if the key is there
	for(int i = 0; i < g_numEntriesTLB; i++) {
		if(((Map*)g_TLB[i])->key == pageNum) {
			return ((Map*)g_TLB[i])->value;
		}
	}
	
	// on failure return -1
	return -1;
}

// page table functions
int pageToFrame(int pageNum) {
	// add it to the TLB
	
	
	// check if its in the page
	if(g_pageTable[pageNum] < 0) {
		g_pageTable[pageNum] = g_numEntriesPageTable * 256;
		g_numEntriesPageTable++;
		g_pageFaults++;
		return g_pageTable[pageNum];
	}
	
	// return the page table
	else {
		return g_pageTable[pageNum];
	}
	
	return -1;
}

// user interface functions
char* parseUserData(int argc, char** argv) {
	// check if the number of parameters is correct
	if(argc > 2 || argc < 2) {
		printUsage(argv[0]);
	}

	// check if file exists
	FILE* fp = fopen(argv[1], "r");
	if(!fp) {
		printUsage(argv[0]);
	}
	fclose(fp);
	
	
	return argv[1];
}

void printUsage(char* programName) {
	fprintf(stderr, "usage: %s filenameoflogicaladdresses\n", programName);
	exit(EXIT_FAILURE);
}
