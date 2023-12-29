// FILE: pageReplacementSimulator.cpp
// J Hall, K Goebel, M Baird, Transy U
// CS 3074, Fall 2023
//
//
//	Implementation for Page Replacement Simulator Class
//
//

#include "pageReplacementSimulator.h"
using namespace std;

PageReplacementSimulator::PageReplacementSimulator (void) {
  nextFreeFrame = 0;
  pageFaultCount = 0;
}

PageReplacementSimulator::~PageReplacementSimulator (void) {
}

void PageReplacementSimulator::simulation (queue<int>& addressQueue, string pid, PageTable& pageTable, int frames, int pages, int frameSize, bool verbose, char* type) {
  if (!strcmp(type, RANDOM)) {
    srand(time(NULL));
  }
  
  if (!strcmp(type, MFU)) {
    frequencyOfUse.assign(pages, 0);
  }
  
  while (!addressQueue.empty()) {
    currentAddress = addressQueue.front();
    addressQueue.pop();
    currentPageNum = (currentAddress/frameSize);
    if (!pageAlreadyInMemory(pageTable)) {
      if (verbose) {
        cout << endl << "Page " << currentPageNum << " is not currently in Memory! Searching for free frames..." << endl;
      }
        
      if (freeFrames(frames, pageTable)) {
        if (verbose) {
          printFreeFrameMessage();
        }
        
        pageTable.updateFreeFrame(currentPageNum, nextFreeFrame);
        victimSelectionQueue.push(currentPageNum);
        if (!strcmp(type, MFU)) {
          frequencyOfUse[currentPageNum] = (frequencyOfUse[currentPageNum] + 1);
        }
        
        ++nextFreeFrame;
      }
      
      else {
        selectVictim(type, frames);
        if (verbose) {
          printNoFreeFrameMessage(pageTable);
        }
        
        pageTable.updateVictimFrame(currentPageNum, victim);
        victimSelectionQueue.push(currentPageNum);
        if (!strcmp(type, MFU)) {
          frequencyOfUse[currentPageNum] = (frequencyOfUse[currentPageNum] + 1);
        }
      }
    }
    
    else if (!strcmp(type, LRU)) {
      moveCurrentPageToBackOfVictimSelectionQueue();
    }
    
    else if (!strcmp(type, MFU)) {
      frequencyOfUse[currentPageNum] = (frequencyOfUse[currentPageNum] + 1);
    }
  }
  
  cout << endl << "***** " << pid << " finished executing with a total of " << pageFaultCount << " page faults! *****" << endl;
}

bool PageReplacementSimulator::pageAlreadyInMemory (PageTable& pageTable) {
  if (pageTable.pageTable[currentPageNum].validBit) {
    return true;
  }
  
  ++pageFaultCount;
  return false;
}

bool PageReplacementSimulator::freeFrames (int frames, PageTable& pageTable) {
  pagesInMemory = 0;
  for (int i=0; i < pageTable.pageTable.size(); ++i) {
    if (pageTable.pageTable[i].validBit) {
      ++pagesInMemory;
    }
  }
  
  if (pagesInMemory < frames) {
    return true;
  }
  
  return false;
}

void PageReplacementSimulator::printFreeFrameMessage (void) {
  cout << endl << "Nice! Found a free frame!" << endl
       << endl << "Page " << currentPageNum << " stored in the first free frame: frame " << nextFreeFrame << "!" << endl
       << endl << "Now have to update page table!" << endl;
}

void PageReplacementSimulator::printNoFreeFrameMessage (PageTable pageTable) {
  cout << endl << "Uh oh, no free frames! Some page in Memory must be removed to make room!" << endl
       << endl << "Page " <<  victim << " was just removed from Memory to make room for page " << currentPageNum << "!" << endl
       << endl << "Page " << currentPageNum << " stored in frame " << pageTable.pageTable[victim].frameNumber << "!" << endl
       << endl << "Now to update the page table!" << endl;
}

int PageReplacementSimulator::selectVictim (char* type, int frames) {
  if (!strcmp(type, RANDOM)) {
    randomizeVictimSelection(frames);
  }
  
  else if (!strcmp(type, MFU)) {
    determineHighestFrequency();
    frequencyOfUse[victim] = 0;
    removeMFUPageFromMemory();
    return victim;
  }
  
  victim = victimSelectionQueue.front();
  victimSelectionQueue.pop();
  return victim;
}

void PageReplacementSimulator::moveCurrentPageToBackOfVictimSelectionQueue (void) {
  while(victimSelectionQueue.front() != currentPageNum) {
    tempQueue.push(victimSelectionQueue.front());
    victimSelectionQueue.pop();
  }
      
  victimSelectionQueue.pop();
  while(!victimSelectionQueue.empty()) {
    tempQueue.push(victimSelectionQueue.front());
    victimSelectionQueue.pop();
  }
      
  tempQueue.push(currentPageNum);
  tempQueue.swap(victimSelectionQueue);
}

void PageReplacementSimulator::randomizeVictimSelection (int frames) {
  victimIndex = (rand() % (frames - 1));  	// figured out how to set the range of rand from source https://www.digitalocean.com/community/tutorials/random-number-generator-c-plus-plus, accessed Nov 13, 2023. 
  memoryShuffler(frames); 
  for (int i=0; i < victimIndex; ++i) {
    firstInQueue = victimSelectionQueue.front();
    victimSelectionQueue.pop();
    victimSelectionQueue.push(firstInQueue);
  }
}

void PageReplacementSimulator::memoryShuffler (int frames) {
  timesShuffled = (rand() % 10);
  for (int i=0; i < timesShuffled; ++i) {
    for (int i=0; i < frames; ++i) {
      firstInQueue = victimSelectionQueue.front();
      victimSelectionQueue.pop();
      secondInQueue = victimSelectionQueue.front();
      victimSelectionQueue.pop();
      victimSelectionQueue.push(secondInQueue);
      victimSelectionQueue.push(firstInQueue);
    } 
  }
}

void PageReplacementSimulator::determineHighestFrequency (void) {
  highestFrequency = 0;
  for(int i=0; i < frequencyOfUse.size(); ++i) {
    if (frequencyOfUse[i] > highestFrequency) {
      highestFrequency = frequencyOfUse[i];
      victim = i;
    }
      
    else if ((frequencyOfUse[i] > 0) && (frequencyOfUse[i] == highestFrequency)) {
      potentialVictim = i;
      defaultToFIFOVictimSelection();
    }
  }
}

void PageReplacementSimulator::defaultToFIFOVictimSelection (void) {
  while((victimSelectionQueue.front() != victim) && (victimSelectionQueue.front() != potentialVictim)) {
    tempQueue.push(victimSelectionQueue.front());
    victimSelectionQueue.pop();
  }
  
  victim = victimSelectionQueue.front(); 
  tempQueue.push(victim);   
  victimSelectionQueue.pop();
  while(!victimSelectionQueue.empty()) {
    tempQueue.push(victimSelectionQueue.front());
    victimSelectionQueue.pop();
  }

  tempQueue.swap(victimSelectionQueue);
}

void PageReplacementSimulator::removeMFUPageFromMemory (void) {
  while(victimSelectionQueue.front() != victim) {
    tempQueue.push(victimSelectionQueue.front());
    victimSelectionQueue.pop();
  }
      
  victimSelectionQueue.pop();
  while(!victimSelectionQueue.empty()) {
    tempQueue.push(victimSelectionQueue.front());
    victimSelectionQueue.pop();
  }

  tempQueue.swap(victimSelectionQueue);
}
