// FILE: randomScheduler.h
// J Hall, K Goebel, M Baird, Transy U
// CS 3074, Fall 2023
//
//     Interface for Random Scheduler Class
//
#ifndef    RANDOMSCHEDULER_H
#define    RANDOMSCHEDULER_H

#include <iostream>
#include <queue>
#include "pageTable.h"
using namespace std;

class RandomScheduler {

 public:
   // public member functions

     // constructor
   RandomScheduler (void);

     // destructor
   ~RandomScheduler (void);

     // mutators
       // simulator
   void simulator (queue<int>& addressQueue, string pid, int frames, int frameSize, PageTable& pageTable, bool verbose);
   
       // pageAlreadyInMemory
   bool pageAlreadyInMemory (int currentPageNum, PageTable& pageTable);
       
       // freeFrames
   bool freeFrames (int frames, PageTable& pageTable);
   
       // selectVictim
   int selectVictim (int frames);
   
       // allocatedQueueShuffler fn takes in nothing and shuffles the allocated queue by popping and pushing for a psuedo-random amount of times between 0 and 10 times. Returns nothing
         // included this because, through cout stmts, noticed that our previous way of trying to do this was generating correct results but wasn't generating all possibilities, calling this function in selectVictim() ensures all possibilities are eventually generated.
   void memoryShuffler (int frames);
    
 private:
   // private member variables
   int currentAddress, currentPageNum, pageFaultCount, pagesInMemory, nextFreeFrame, victimPageNum, victimIndex, timesShuffled, firstInQueue, secondInQueue, victim;
   queue<int> inMemoryQueue;
       
};

#endif