/**
	Student Name			Will Pringle
	Student ID #			1056636
**/
/// includes
#include <stdlib.h>

/// page table structure global variable
// holds the hashmap between all page numbers and all frame numbers
int* g_pageTable = 0;

/// global member variables
// number of entries
int g_numEntriesPageTable = 0;
// numbre of page faults
int g_pageFaults = 0;

/// functions
/** Function to retrieve a frame from a page, adds a page frame map if not there
 *@pre page table is not NULL and has memory allocated
 *@post new map added to page table if not existed before, otherwise unchanged
 *@return frame number that a page number maps to
 *@param pageNum - the page number for the new map
**/
int pageToFrame(int pageNum);