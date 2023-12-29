// FILE: randomScheduler.cpp
// J Hall, K Goebel, M Baird, Transy U
// CS 3074, Fall 2023
//
//
//  Implementation for Random Scheduler Class
//
//

#include "randomScheduler.h"
using namespace std;

RandomScheduler::RandomScheduler (void) {
  nextFreeFrame = 0;
  pageFaultCount = 0;
}

RandomScheduler::~RandomScheduler (void) {
}

void RandomScheduler::simulator (queue<int>& addressQueue, string pid, int frames, int frameSize, PageTable& pageTable, bool verbose) {
  srand(time(NULL));
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
        victimPageNum = selectVictim(frames);
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
  }
  
  cout << endl << "***** " << pid << " finished executing with a total of " << pageFaultCount << " page faults! *****" << endl;
}

bool RandomScheduler::pageAlreadyInMemory (int currentPageNum, PageTable& pageTable) {
  if (pageTable.pageTable[currentPageNum].validBit) {
    return true;
  }
  
  ++pageFaultCount;
  return false;
}

bool RandomScheduler::freeFrames (int frames, PageTable& pageTable) {
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

int RandomScheduler::selectVictim (int frames) {
  victimIndex = (rand() % (frames - 1));    // figured out how to set the range of rand from source https://www.digitalocean.com/community/tutorials/random-number-generator-c-plus-plus, accessed Nov 13, 2023. 
  memoryShuffler(frames); 
  for (int i=0; i < victimIndex; ++i) {
    firstInQueue = inMemoryQueue.front();
    inMemoryQueue.pop();
    inMemoryQueue.push(firstInQueue);
  }

  victim = inMemoryQueue.front();
  inMemoryQueue.pop();
  return victim;
}

void RandomScheduler::memoryShuffler (int frames) {
  timesShuffled = (rand() % 10);
  for (int i=0; i < timesShuffled; ++i) {
    for (int i=0; i < frames; ++i) {
      firstInQueue = inMemoryQueue.front();
      inMemoryQueue.pop();
      secondInQueue = inMemoryQueue.front();
      inMemoryQueue.pop();
      inMemoryQueue.push(secondInQueue);
      inMemoryQueue.push(firstInQueue);
    }
  }
}