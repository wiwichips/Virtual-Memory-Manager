/**
	Student Name			Will Pringle
	Student ID #			1056636
**/
/// includes
#include <stdlib.h>

/// TLB data structure global variable
// holds the maps from page number to frame number
void** g_TLB = 0;

/// global member variables
// number of entries
int g_numEntriesTLB = 0;
 // number of tlb hits
int g_hitsTLB = 0;

/// structures
// map with a key and a value
typedef struct Map {
   // represents the page number
   int key;
   // represents the frame number
   int value;
} Map;

/// functions

/** Function to add a new key-value mapping to a TLB.
 *@pre TLB is not NULL and has memory allocated
 *@post TLB has new map, if TLB was full: the last map is deleted, num entries++
 *@return none
 *@param pageNum - the page number for the new map
 *@param frameNum - the frame number that the page number maps to
**/
void addToTLB(int pageNum, int frameNum);

/** Function to get the frame number from a page number map.
 *@pre TLB is not NULL and has memory allocated
 *@post TLB is not modifed, g_hitsTLB is increased if the page number maps
 *@return if it maps, the frame number the page maps to, otherwise negative valu
 *@param pageNum - the page number that maps to the frame number the user wants
**/
int getFrameTLB(int pageNum);