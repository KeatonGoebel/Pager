// FILE: MFUScheduler.h
// J Hall, K Goebel, M Baird, Transy University
// Fall 2023
//
//     Interface for MFU Scheduler Class

#ifndef    MFUSCHEDULER_H
#define    MFUSCHEDULER_H

#include <iostream>
#include <queue>
#include "pageTable.h"
using namespace std;

class MFUScheduler {

 public:

    // constructor
   MFUScheduler (void);

    // destructor
   ~MFUScheduler (void);

    // mutators
      // The simulator function acts as a wrapper by taking in several different parameters and using them to call different functions in the LRUScheduler class.
      // Simulator goes through the address queue of pages and if the page is not in memory, it either puts the page into a free frame or finds a victim frame and removes it.
      // The victim is the most frequently used pages in memory. If the page is not currently in memory, it adds the page to an in memory queue and removes 
      // the page from that queue when it is time to select a victim
   void simulator (queue<int>& addressQueue, string pid, int frames, int frameSize, PageTable& pageTable, bool verbose);
   
     // The pageAlreadyInMemory function takes in a page number and a page table and returns true or false if the page is currently in memory or not
   bool pageAlreadyInMemory (int currentPageNum, PageTable& pageTable);
       
       // The freeFrames function takes number of frames and a page table and returns true or false if a free frame exists
   bool freeFrames (int frames, PageTable& pageTable);
   
       // selectVictim returns the page number of the selected page number to be the victim
   int selectVictim (void);
    
 private:
   int currentAddress, currentPageNum, pageFaultCount, pagesInMemory, nextFreeFrame, victimPageNum, victim;
   queue<int> inMemoryQueue;
       
};

#endif