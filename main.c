#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structs
typedef struct Map {
   int key;
   int value;
} Map;

// global data structures
Map** TLB = NULL;
int NUM_TLB_ENTRIES = 0;


int* PAGE_TABLE = NULL;
int NUM_PAGE_TABLE_ENTRIES = 0;


// global variables
int SIZE = 256;
char* BACKING_STORE = "BACKING_STORE.bin";
int PAGE_FAULTS = 0;

// function protos
int getPageDetails(int logicalAddress, int pageSize, int* pageNum, int* offset);
char getValueFromStore(char* filename, int address);
int getFrameNumber(int pageNum);

// helper functions
void driver();
int* createFileArray(char* filename, int* numNums);
void init(int TLBsize, int pageTableSize);
void cleanUp();

int logBaseTwo(int x, int ret);

// page table functions
int pageToFrame(int pageNum); // returns frame from page number

// hashmap functions
void hashPut(int key, int value);
int hashGet(int key);
void hashPrint();

// testing
void test1();

int main() {
	init(16, 256);
	driver();
	// test1();
	cleanUp();
	return 0;
}

void driver() {
	// initialize variables
	int numNums = 0;
	int* nums = createFileArray("addresses.txt", &numNums);
	int pageNum = 0;
	int pageOff = 0;
	
	int tmp = 0;
	
	for(int i = 0; i < numNums; i++) {
		getPageDetails(nums[i], 256, &pageNum, &pageOff);
		
		tmp = pageToFrame(pageNum);
		
		// printf("Virtual address: %d Physical address: %d Value: %d", nums[i], tmp + pageOff, getValueFromStore(BACKING_STORE, nums[i]));
		// puts("");
	}
	
	printf("Number of Translated Addresses = %d\n", numNums);
	printf("Page Faults = %d\n", PAGE_FAULTS);
	printf("Page Fault Rate = %.3f\n", ((float) PAGE_FAULTS) / ((float) numNums));
	printf("TLB Hits = %d\n", 0);
	printf("TLB Hit Rate = %.3f\n", 0.0);
	
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
	int frame = -2;
	
	// logicalAddress version of pageNum in case need to access virtual memory
	int logicalAddress = pageNum << logBaseTwo(SIZE, 0);
	
	/// first check the TLB
	// return TLB if hit, otherwise continue
	
	/// check the page table
	// if element is not in page table
	if(hashGet(pageNum) < 0) {						// PAGE FAULT
		// get it from backing store and put it in PAGE TABLE
		hashPut(pageNum, getValueFromStore(BACKING_STORE, logicalAddress));
	}
	// if element is in page table
	else {
		
	}
	
	// get it from the page table
	frame = hashGet(pageNum);
	
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

		// printf("%i ## %d \n", i, nums[i]);
	}
	
	// close file
	fclose(fp);
	
	// set the numNums to the size of the file ( minus 1 )
	*numNums = i - 1;
	
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
	TLB = calloc(TLBsize, sizeof(Map));
	
	PAGE_TABLE = calloc(SIZE, sizeof(int));
	
	// initialize all values in page table to -1
	memset(PAGE_TABLE, -1, SIZE * sizeof(int));
}

void cleanUp() {
	for(int i = 0; i < NUM_TLB_ENTRIES; i++) {
		free(TLB[i]);
	}
	free(TLB);
	free(PAGE_TABLE);
}


// page table functions
int pageToFrame(int pageNum) {
	// check if its in the page
	if(PAGE_TABLE[pageNum] < 0) {
		PAGE_TABLE[pageNum] = NUM_PAGE_TABLE_ENTRIES * 256;
		NUM_PAGE_TABLE_ENTRIES++;
		PAGE_FAULTS++;
		return PAGE_TABLE[pageNum];
	}
	
	// return the page table
	else {
		return PAGE_TABLE[pageNum];
	}
	
	return -1;
}





// hashmap functions
void hashPut(int key, int value) { // BROKEN TODO
	// create new map
	Map* newMap = calloc(1, sizeof(Map));
	newMap->key = key;
	newMap->value = value;
	
	// temp variables
	Map* temp = newMap;
	Map* temp2 = NULL;
	
	/// add it to the queue
	// if its the first, add it
	if(!NUM_TLB_ENTRIES) {
		TLB[0] = newMap;
	}
	
	// append without removing if there is space
	for(int i = 0; i < NUM_TLB_ENTRIES; i++) {
		temp2 = TLB[i];
		
		TLB[i] = temp;
		
		temp = temp2;
	}
	
	free(temp2);
	
	if(NUM_TLB_ENTRIES < 16) {
		NUM_TLB_ENTRIES++;
	}
	
}

int hashGet(int key) {
	
	for(int i = 0; i < NUM_TLB_ENTRIES; i++) {
		if(key == TLB[i]->key) {
			return TLB[i]->value;
		}
	}
	
	return -1;
}

void hashPrint() {
	for(int i = 0; i < NUM_TLB_ENTRIES; i++) {
		printf("Key = %d\tValue = %d\n", TLB[i]->key, TLB[i]->value);
	}
}
