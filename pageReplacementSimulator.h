// FILE: pageReplacementSimulator.h
// J Hall, K Goebel, M Baird, Transy University
// Fall 2023
//
//  Interface for Page Replacement Simulator Class
//

#ifunctiondef    PAGEREPLACEMENTSIMULATOR_H
#define    PAGEREPLACEMENTSIMULATOR_H

#include <iostream>
#include <queue>
#include <string.h>
#include "symConsts.h"
#include "pageTable.h"
using namespace std;

class PageReplacementSimulator {

 public:

    // constructor initializes nextFreeFrame and pageFaultCount to 0
   PageReplacementSimulator (void);

    // destructor
   ~PageReplacementSimulator (void);

    // mutator
      // simulator function takes in the queue of address, the pid of the process, the page table, and the value for the number of frames, the number of pages, the 
      // frame size, the verbose flag, and the type inputted on the command line (or the defaults). This function seeds for rand() if type is Random, 
      // or initializes a vector of size equal to the number of pages to track frequency of use if type is MFU, or does nothing before entering a loop of 
      // the following until no addresses left in the queue: get an address from the address queue, pop it, calculate what page the address is in, determine if the 
      // page is already in memory. If page is in memory and type is LRU, call moveCurrentPageToBackOfVictimSelectionQueue(). If page is in memory and type is MFU, 
      // increment the frequency of the current page. If page is in memory and type is FIFO or Random, do nothing. If page is not in memory, check for free frames.
      // If a free frame exists, call updateFreeFrame() on the page table, push the current page number onto the victimSelectionQueue, increment nextFreeFrame, and 
      // if type is MFU, increment the frequency of the current page. If no free frames, call select victim, then call updateVictimFrame() on the page table, and 
      // if type is MFU, increment the frequency of the current page. Returns nothing
   void simulation (queue<int>& addressQueue, string pid, PageTable& pageTable, int frames, int pages, int frameSize, bool verbose, char* type);

 private:
   int currentAddress, currentPageNum, pageFaultCount, pagesInMemory, nextFreeFrame, victimIndex, timesShuffled, firstInQueue, secondInQueue, highestFrequency, victim, potentialVictim;
   queue<int> victimSelectionQueue, tempQueue;
   vector<int> frequencyOfUse;
       
    // mutators
      // pageAlreadyInMemory function takes in the page table. This function checks the page table to see if the valid bit for the current page is set to true, incrementing 
      // the pageFaultCount if it is instead set to false. Returns true if current page's valid bit is set to true, false if not
   bool pageAlreadyInMemory (PageTable& pageTable);
       
      // freeFrames function takes in the value for the number of frames inputted on the command line (or the default if not inputted), and the page table. This 
      // function sets pagesInMemory to 0, then checks the page table, incrementing pagesInMemory each time an entry has a valid bit that is set to true. Returns 
      // true if pagesInMemory is less than the number of frames, returns false otherwise
   bool freeFrames (int frames, PageTable& pageTable);
   
      // selectVictim function takes in the value for the type and frames variables that were specified in the command line (or the defaults if not inputted). 
      // This function checks if anything special needs to be done due to the type, then for all types other than MFU, this function sets victim to the first 
      // element in the victimSelectionQueue, and returns victim. If type is set to Random this function calls randomizeVictimSelection, then proceeds as 
      // described. If type is set to MFU, this function calls determineHighestFrequency(), sets the victim's frequency to 0, calls removeMFUPageFromMemory() 
      // and returns victim.
   int selectVictim (char* type, int frames);
   
      // moveCurrentPageToBackOfVictimSelectionQueue function takes in nothing. This function pushes everything within the victimSelectionQueue into a temporary 
      // queue while also popping everything within the victimSelectionQueue, ensuring that the current page number is the last to be pushed into the temporary 
      // queue. The queues are then swapped so that victimSelectionQueue now has the current page number as it's last element, and the temporary queue is empty. Returns nothing
   void moveCurrentPageToBackOfVictimSelectionQueue (void);
   
      // randomizeVictimSelection function takes in the value for the number of frames inputted on the command line (or the default if not inputted). This 
      // function calls rand() to pick a random frame from memory, then calls memoryShuffler() to shuffle the frames in memory, then ensures that the random frame 
      // picked by rand() is the first in the queue by pushing and popping the victimSelectionQueue a number of times equal to the frame picked by rand(). Returns nothing
   void randomizeVictimSelection (int frames);
   
      // memoryShuffler function takes in the value for the number of frames inputted on the command line (or the default if not inputted) and shuffles the 
      // victimSelectionQueue by popping and pushing for a psuedo-random amount of times between 0 and 10 times. Returns nothing.
   void memoryShuffler (int frames);

      // determineHighestFrequency function takes in nothing. This function checks the entire frequencyOfUse vector to find the most frequently used page that's 
      // currently in memory, setting the victim to this page number. If a page with the highest frequency has an equivalent frequency as another page in memory, 
      // this function calls defaultToFIFOVictimSelection(). Returns nothing
   void determineHighestFrequency (void);
   
      // defaultToFIFOVictimSelection function takes in nothing. This function checks which of the two pages with equivalent frequency was first to come into 
      // memory by pushing everything from the victimSelectionQueue into a temporary queue until either of those page numbers are the first in the queue, saving 
      // this page number as the victim, pushing the victim into the temporary queue as well, and popping everything from the victimSelectionQueue before swapping 
      // the queues. Swapping the queues ensures that the victimSelectionQueue is exactly how it was before this check, and that the temporary queue is left empty. Returns nothing
   void defaultToFIFOVictimSelection (void);
   
      // removePageFromMemory function takes in the page number of the victim to be removed. This function pushes everything within the victimSelectionQueue 
      // into a temporary queue except for the victim page number, while also popping everything within the victimSelectionQueue. The queues are then swapped 
      // so that the victimSelectionQueue now doesn't have the victim in it, and the temporary queue is empty. Returns nothing
   void removeMFUPageFromMemory (void); 
   
    // accessors
      // printFreeFrameMessage function takes in nothing. This function prints a message stating that a free frame exits, and what frame the current page is
      // getting stored in. Returns nothing
   void printFreeFrameMessage (void);
   
      // printNoFreeFrameMessage function takes in the page table. This function prints a message stating that no free frames are available, which page was 
      // removed from memory to make a free frame, and what frame the current page is getting stored in. Returns nothing
   void printNoFreeFrameMessage (PageTable pageTable);    
};

#endif
