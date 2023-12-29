// FILE: pager.cpp
// J Hall, K Goebel, M Baird, Transy University
// Fall 2023
//

#include "pager.h"
using namespace std;  

int main (int argc, char** argv) {
  char type[CHAR_MAX] = TYPE_DEFAULT;
  char file[CHAR_MAX] = INPUT_FILE_DEFAULT;
  int frames = FRAMES_DEFAULT;
  int pages = PAGES_DEFAULT;
  int frameSize = FRAMESIZE_DEFAULT;
  bool verbose = VERBOSE_DEFAULT;
  bool typeSpecified = false;
  bool framesSpecified = false;
  bool pagesSpecified = false;
  bool frameSizeSpecified = false;
  bool useDefaultFile = false;
  string pidNum = "";
  ifstream inFile;
  double addressNum;
  string pid, address;
  queue<int> addressQueue;
  PageReplacementSimulator simulator;
  
  argErrorChecker (argc, argv, useDefaultFile, file, inFile, type, frames, pages, frameSize, typeSpecified, framesSpecified, pagesSpecified, frameSizeSpecified, verbose);
  inFile >> pid;
  pidChecker(pid, pidNum); 
  inFile >> address;
  while (!inFile.eof()) {
    ensureAddressIsValid(address, addressNum, frameSize, pages);
    addressQueue.push(addressNum);
    inFile >> address;
  }
  
  PageTable pageTable(pages);     
  simulator.simulation(addressQueue, pid, pageTable, frames, pages, frameSize, verbose, type);
  return 0;
}

void argErrorChecker (int argCount, char** argList, bool& useDefaultFile, char* file, ifstream& inFile, char* type, int& frames, int& pages, int& frameSize, bool& typeSpecified, bool& framesSpecified, bool& pagesSpecified, bool& frameSizeSpecified, bool& verbose) {
  for (int i=1; i < argCount; ++i) {
    if (checkIfArgThatNeedsSpecifier((argCount - 1), i, argList[i], argList, useDefaultFile, type, frames, pages, frameSize, typeSpecified, framesSpecified, pagesSpecified, frameSizeSpecified)) {
    }
    
    else if (!strcmp(argList[i], VERBOSE)) {
      verbose = true;
    }
    
    else if ((i == (argCount - 1)) && (!useDefaultFile)) {
      strcpy(file, argList[(i)]);
    }
    
    else if (checkIfLooseSpecifierOrTypo(i, argList)) {
       exit(1);
    }
  }
  
  inFile.open(file);
  if (!inFile.is_open()) {
    cout << endl << "Unable to open file. Please retry." << endl
         << "Either the file is not in the directory, or the specified file name is invalid." << endl << endl;
    exit(1);
  }
}

bool checkIfArgThatNeedsSpecifier (int lastArgIndex, int currentArgIndex, const char* currentArg, char** argList, bool& useDefaultFile, char*& type, int& frames, int& pages, int& frameSize, bool& typeSpecified, bool& framesSpecified, bool& pagesSpecified, bool& frameSizeSpecified) {
  if (!strcmp(currentArg, TYPE)) {
    if (typeSpecified) {
      printAlreadySpecifiedError(currentArg);
    }
    
    typeSpecified = true;
    if (argIsLastArg(lastArgIndex, currentArgIndex)) {
      cout << endl << "Scheduling type";
      printArgNotSpecifiedError(currentArg);
    }
    
    strcpy(type, argList[(currentArgIndex + 1)]);
    if (nextArgIsLastArg(lastArgIndex, (currentArgIndex + 1))) {
      useDefaultFile = true;
    }
    
    if (invalidType(type)) {
      cout << endl << "Specified scheduling type is not a valid type. Please retry." << endl
           << "Ensure that this value is specified directly after argument '--type', separated by a space, and is one of the following types: 'FIFO', 'LRU', 'MFU', 'Random'." << endl << endl;
        exit(1);
    } 
    
    return true;
  } 
   
  else if (!strcmp(currentArg, FRAMES)) {
    if (framesSpecified) {
      printAlreadySpecifiedError(currentArg);
    }
    
    framesSpecified = true;
    if (argIsLastArg(lastArgIndex, currentArgIndex)) {
      cout << endl << "Number of frames";
      printArgNotSpecifiedError(currentArg);
    }
    
    frames = atoi(argList[(currentArgIndex + 1)]);
    if (nextArgIsLastArg(lastArgIndex, (currentArgIndex + 1))) {
      useDefaultFile = true;
    }
    
    if ((frames > MAX_FRAMES) || (frames < 1)) {
      cout << endl << "Number of frames specified is invalid. Please retry." << endl
           << "Ensure that this value is specified directly after argument '--frames', separated by a space, and that this value is ONLY an integer within the range of 1 to " << MAX_FRAMES << "." << endl << endl;
      exit(1);  
    }
   
    return true;
  } 
   
  else if (!strcmp(currentArg, PAGES)) {
    if (pagesSpecified) {
      printAlreadySpecifiedError(currentArg);
    }
    
    pagesSpecified = true;
    if (argIsLastArg(lastArgIndex, currentArgIndex)) {
      cout << endl << "Number of pages";
      printArgNotSpecifiedError(currentArg);
    }
    
    pages = atoi(argList[(currentArgIndex + 1)]);
    if (nextArgIsLastArg(lastArgIndex, (currentArgIndex + 1))) {
      useDefaultFile = true;
    }
    
    if (pages < 1) {
      cout << endl << "Number of pages specified is invalid. Please retry." << endl
           << "Ensure that this value is specified directly after argument '--pages', separated by a space, and that this value is ONLY an integer greater than 0." << endl << endl;
      exit(1);
    }
    
    return true;
  }
   
  else if (!strcmp(currentArg, FRAMESIZE)) {
    if (frameSizeSpecified) {
      printAlreadySpecifiedError(currentArg);
    }
    
    frameSizeSpecified = true;
    if (argIsLastArg(lastArgIndex, currentArgIndex)) {
      cout << endl << "Frame size";
      printArgNotSpecifiedError(currentArg);
    }
    
    if (valueIsFloat(currentArg)) {
      cout << endl << "Size of frame specified is of invalid type. Please retry." << endl
           << "Ensure that this value is ONLY an integer." << endl << endl;
      exit(1);
    }
    
    frameSize = atoi(argList[(currentArgIndex + 1)]);
    if (nextArgIsLastArg(lastArgIndex, (currentArgIndex + 1))) {
      useDefaultFile = true;
    }
    
    if (frameSize < 1) {
      cout << endl << "Size of frame specified is invalid. Please retry." << endl
           << "Frame sizes are always a power of the number two. Ensure that this is the case for the input." << endl << endl;
      exit(1);
    } 
    
    if (!powerOfTwo(frameSize)) {
      cout << endl << "Size of frame specified is invalid. Please retry." << endl
           << "Frame sizes are always a power of the number two. Ensure that this is the case for the input." << endl << endl;
      exit(1);
    }
    
    return true;
  }
   
  return false;
}

void printAlreadySpecifiedError (const char* currentArg) {
  cout << endl << "Argument '" << currentArg << "' can only be specified once. Please retry." << endl << endl;
  exit(1); 
}

bool argIsLastArg (int lastArgIndex, int currentArgIndex) {
  if (currentArgIndex == lastArgIndex) {
    return true;
  }
  
  return false;
}

void printArgNotSpecifiedError (const char* arg) {    
  cout << " was not specified. Please retry." << endl << "Ensure that this value is specified directly after argument '" << arg << "', separated by a space." << endl << endl;
  exit(1);
}

bool valueIsFloat (const char* value) {
  int strLength = strlen(value);
  for (int i=0; i < strLength; ++i) {
    if ((value[i] == '.') || (value[i] == ',')) {
      return true;
    }
  }
  
  return false;
}

bool nextArgIsLastArg (int lastArgIndex, int nextArgIndex) {
  if (nextArgIndex == lastArgIndex) {
    return true;
  }
  
  return false;
}

bool invalidType (string type) {
  if ((strcmp(type.c_str(), TYPE_DEFAULT)) && (strcmp(type.c_str(), LRU)) && (strcmp(type.c_str(), MFU)) && (strcmp(type.c_str(), RANDOM))) {
    return true;
  }
  
  return false;
}

bool powerOfTwo (int frameSize) {
  if ((ceil(log2(frameSize))) == (floor(log2(frameSize)))) {
    return true;
  }
  
  else {
    return false;
  }
}

bool checkIfLooseSpecifierOrTypo (int currentIndex, char** argList) {
  if ((!strcmp(argList[(currentIndex - 1)], TYPE)) || (!strcmp(argList[(currentIndex - 1)], FRAMES)) || (!strcmp(argList[(currentIndex - 1)], PAGES)) || (!strcmp(argList[(currentIndex - 1)], FRAMESIZE))) {
    return false;
  } 
   
  else {
    cout << endl << "No loose specifiers or typos allowed.  Please retry." << endl
         << "Ensure that any specifier arguments are directly after the appropriate argument of form '--<argumentType>', separated by a space, and that a file argument is the LAST in the argument list." << endl << endl;
    return true;
  }
}

void pidChecker (string pid, string pidNum) {
  if (pid[0] != 'P') {
    printFileFormatError();
  }
  
  if (pid[1] != '_') {
    printFileFormatError();
  }
  
  for (int i=2; i < pid.length(); ++i) {
    pidNum = pidNum + pid[i];
  }
  
  if ((!stringIsZero(pidNum)) && (!atoi(pidNum.c_str()))) {
    printFileFormatError();
  }
}

void printFileFormatError (void) {
  cout << endl << "This file is not of proper format. Please retry." << endl
         << "Proper files fit the form: P_<pidInteger> followed by addresses in the form of integers, each separated by a space." << endl << endl;
    exit(1);
}

void ensureAddressIsValid (string address, double& addressNum, int& frameSize, int& pages) {
  if (valueIsFloat(address.c_str())) {
    printFileFormatError();
  }
  
  addressNum = atof(address.c_str());
  if ((!stringIsZero(address)) && (!addressNum)) {
    printFileFormatError();
  }  
    
  if ((addressNum/frameSize) > pages) {
    cout << endl << "Address " << address << " is not a valid address. Please retry." << endl
         << "Ensure that for all addresses of the input file, when the address is divided by the framesize, it does not exceed the number of pages." << endl << endl;
    exit(1); 
  }
}

bool stringIsZero (string str) {
  for (int i=0; i < str.length(); ++i) {
    if (str[i] != '0') {
      return false;
    }
  }
  
  return true;
}