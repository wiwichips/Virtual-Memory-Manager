#include <stdio.h>
#include <stdlib.h>

// returns a malloced array of ints 
int* createFileArray(char* filename, int* numNums);
void getPageDetails(int logicalAddress, int pageSize, int* number, int* offset);
int logBaseTwo(int x, int ret);

int main() {
	
	int numNums = 0;
	int* nums = createFileArray("will.txt", &numNums);
	int pageNum = 0;
	int pageOff = 0;
	
	for(int i = 0; i < numNums; i++) {
		printf("%i ~~ %d \n", i, nums[i]);
		getPageDetails(nums[i], 1024, &pageNum, &pageOff);
	}
		
	free(nums);
	
	return 0;
}

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

void getPageDetails(int logicalAddress, int pageSize, int* number, int* offset) {	
	// check if pointers are NULL or pageSize is invalid
	if(!number || !offset || (pageSize % 2)) { return; }
	
	// convert pageSize to number of bits
	int numBits = logBaseTwo(pageSize, 0);
	
	*number = logicalAddress >> numBits; // truncate first 10 bits

	*offset = logicalAddress - ((logicalAddress >> numBits) << numBits);
	
	
	printf("number = %d\n", *number);
	printf("offset = %d\n\n", *offset);
	
	return;
}

int logBaseTwo(int x, int ret) {
	if(x == 0) {
		return --ret;
	}
	
	return logBaseTwo(x/2, ++ret);
}



