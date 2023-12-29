// FILE: LRUScheduler.h
// J Hall, K Goebel, M Baird, Transy U
// CS 3074, Fall 2023
//
//     Interface for LRU Scheduler Class
//
#ifndef    LRUSCHEDULER_H
#define    LRUSCHEDULER_H

#include <iostream>
#include <queue>
#include "pageTable.h"
using namespace std;

class LRUScheduler {

 public:
   // public member functions

     // constructor
   LRUScheduler (void);

     // destructor
   ~LRUScheduler (void);

     // mutators
       // The simulator function acts as a wrapper by taking in several different parameters and using them to call different functions in the LRUScheduler class
       // simulator goes through the address queue of pages and if the page is not in memory, it either puts the page into a free frame or finds a victim frame and removes it.
       // The victim is the least recently used page in memory. If the page is currently in memory, it reorganizes the queue to move the page to the back of a in memory queue
   void simulator (queue<int>& addressQueue, string pid, int frames, int frameSize, PageTable& pageTable, bool verbose);
   
       // The pageAlreadyInMemory function takes in a page number and a page table and returns true or false if the page is currently in memory or not
   bool pageAlreadyInMemory (int currentPageNum, PageTable& pageTable);
       
       // The freeFrames function takes number of frames and a page table and returns true or false if a free frame exists
   bool freeFrames (int frames, PageTable& pageTable);
   
       // selectVictim returns the page number of the selected page number to be the victim
   int selectVictim (void);
    
 private:
   // private member variables
   int currentAddress, currentPageNum, pageFaultCount, pagesInMemory, nextFreeFrame, victimPageNum, victim;
   queue<int> inMemoryQueue;
       
};

#endif