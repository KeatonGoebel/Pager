// FILE: LRUScheduler.cpp
// J Hall, K Goebel, M Baird, Transy University
// Fall 2023
//
//
//  Implementation for LRU Scheduler Class
//

#include "LRUScheduler.h"
using namespace std;

LRUScheduler::LRUScheduler (void) {
  nextFreeFrame = 0;
  pageFaultCount = 0;
}

LRUScheduler::~LRUScheduler (void) {
}

void LRUScheduler::simulator (queue<int>& addressQueue, string pid, int frames, int frameSize, PageTable& pageTable, bool verbose) {

  while (!addressQueue.empty()) {
    currentAddress = addressQueue.front();
    addressQueue.pop();
    currentPageNum = (currentAddress/frameSize);

    if (!pageAlreadyInMemory(currentPageNum, pageTable)) {

      if (verbose) {
        cout << endl << "Page " << currentPageNum << " is not currently in Memory! Searching for free frames..." << endl;
      }

      if (freeFrames(frames, pageTable)) {
        if (verbose) {
        cout << endl << "Nice! Found a free frame!" << endl
             << endl << "Page " << currentPageNum << " stored in the first free frame: frame " << nextFreeFrame << "!" << endl
             << endl << "Now have to update page table!" << endl;
        }
        
        pageTable.updateFreeFrame(currentPageNum, nextFreeFrame);
        inMemoryQueue.push(currentPageNum);
        ++nextFreeFrame;
      }
      
      else {
        victimPageNum = selectVictim();
        if (verbose) {
          cout << endl << "Uh oh, no free frames! Some page in Memory must be removed to make room!" << endl
               << endl << "Page " <<  victim << " was just removed from Memory to make room for page " << currentPageNum << "!" << endl
               << endl << "Page " << currentPageNum << " about to be stored in frame " << pageTable.pageTable[victimPageNum].frameNumber << "!" << endl
               << endl << "Now to update the page table!" << endl;
        }
        
        pageTable.updateVictimFrame(currentPageNum, victimPageNum);
        inMemoryQueue.push(currentPageNum);
      }
    }
    else {

      queue<int> tempQueue;
      while(inMemoryQueue.front() != currentPageNum){
        tempQueue.push(inMemoryQueue.front());
        inMemoryQueue.pop();
      }
      inMemoryQueue.pop();

      while(!inMemoryQueue.empty()){
        tempQueue.push(inMemoryQueue.front());
        inMemoryQueue.pop();
      }
      tempQueue.push(currentPageNum);
      tempQueue.swap(inMemoryQueue);
    }

  }
  
  cout << endl << "***** " << pid << " finished executing with a total of " << pageFaultCount << " page faults! *****" << endl;
}

bool LRUScheduler::pageAlreadyInMemory (int currentPageNum, PageTable& pageTable) {
  if (pageTable.pageTable[currentPageNum].validBit) {
    return true;
  }
  
  ++pageFaultCount;
  return false;
}

bool LRUScheduler::freeFrames (int frames, PageTable& pageTable) {
  pagesInMemory = 0;
    for (int i=0; i < pageTable.pageTable.size(); ++i) {
      if (pageTable.pageTable[i].validBit) {
        ++pagesInMemory;
      }
    }
  
    if (pagesInMemory == frames) {
      return false;
    }
  
  return true;
}

int LRUScheduler::selectVictim (void) {
  victim = inMemoryQueue.front();
  inMemoryQueue.pop();
  return victim;
}