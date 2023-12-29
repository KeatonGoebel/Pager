// FILE: pageTable.cpp
// J Hall, K Goebel, M Baird, Transy University
// Fall 2023
//
//
//  Implementation of Page Table Class
//

#include "pageTable.h"
using namespace std;

PageTable::PageTable (int pages) {
  emptyEntry.frameNumber = (-1);
  emptyEntry.validBit = false;
  pageTable.assign(pages, emptyEntry);
}

PageTable::~PageTable (void) {
}

void PageTable::updateFreeFrame (int currentPageNum, int frameNum) {
  pageTable[currentPageNum].frameNumber = frameNum;
  pageTable[currentPageNum].validBit = true;
}

void PageTable::updateVictimFrame (int currentPageNum, int victimPageNum) {
  victimFrame = pageTable[victimPageNum].frameNumber;
  pageTable[victimPageNum].frameNumber = (-1);
  pageTable[victimPageNum].validBit = false;
  pageTable[currentPageNum].frameNumber = victimFrame;
  pageTable[currentPageNum].validBit = true;
}