// FILE: pageTable.h
// J Hall, K Goebel, M Baird, Transy University
// Fall 2023
//
//
//  Interface for the Page Table Class
//

#ifndef    PAGETABLE_H
#define    PAGETABLE_H

#include <vector>
using namespace std;

class PageTable {

 public:
   struct TableEntry {
     int frameNumber;
     bool validBit;
   };
  
  TableEntry emptyEntry; 
  vector<TableEntry> pageTable;

    // constructor takes in the number of pages inputted on the command line (or the default if not inputted) and initializes a page table with a number of 
    // entries equal to the number of pages inputted on command line
    PageTable (int pages);

    // destructor
    ~PageTable (void);
    
    // mutators
      // updateFreeFrame fn takes in the current page number and the frame number of the free frame. This fn sets the frame number of the current page's entry 
      // in the page table to the frame number of the free frame, and sets the valid bit to true for this page's page table entry. Returns nothing
    void updateFreeFrame (int currentPageNum, int frameNum);
      
      // updateVictimFrame fn takes in the current page number and the victim's page number. This fn saves the frame number that the victim page is currently in, 
      // sets the frame number of the victim page's entry in the page table to -1, sets the valid bit to false for the victim page's page table entry, sets the frame number of the current page's entry in the page table to what the victim's frame number had been before it was set to -1, and sets the valid bit to true for the current page's page table entry. Returns nothing
    void updateVictimFrame (int currentPageNum, int victimPageNum);
    
 private:
   int victimFrame;
};

#endif