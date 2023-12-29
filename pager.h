// FILE: pager.h
// J Hall, K Goebel, M Baird, Transy University
// Fall 2023
//
//  Interface for the page replacement simulator project (pager)
//
//  This program takes in a list of argument inputs from the command line and simulates a page replacement algorithm dependent on what the user enters 
//  as the type of page replacement algorithm, as well as the other command line arguments. 
//
//  This program ONLY takes in command line arguments of the form '--type' followed by one of the following appropriate types: FIFO, LRU, MFU or Random, 
//  it takes in the input '--frames' followed by an integer, the input '--pages' followed by an integer, and the input '--framesize' followed by an 
//  integer that must be a power of two. The program also takes in the input '--verbose' but that is optional. This program can also take in a desired input file 
//  to be used in the program, so long as this file's name is the last argument specified on the command line, and it fits the proper form. An input file
//  of proper form will always have ONLY two characters of the form 'P_', followed by an integer, as the first element of the file. All subsequent elements
//  will just be integer values. All of these subsequent integers must yield a result smaller than the number of pages when divided by the integer following 
//  the '--framesize' argument.  
//  Any other inputs to the command line will result in an error message followed by the program exiting.
//
//  Should the user NOT enter any arguments into the command line, each of the appropriate arguments will default to some value. '--type' will deafult to FIFO,
//  '--frames' will default to 3, '--pages' will default to 8, '--framesize' will default to 512, '--verbose' will default to false, and if no input file is
//  specified, the input file will default to pager.in.
//

#ifndef PAGER_H
#define PAGER_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <queue>
#include "symConsts.h"
#include "pageTable.h"
#include "pageReplacementSimulator.h"

using namespace std;
#define TYPE "--type"
#define FRAMES "--frames"
#define PAGES "--pages"
#define FRAMESIZE "--framesize"
#define VERBOSE "--verbose"
#define TYPE_DEFAULT "FIFO"
#define INPUT_FILE_DEFAULT "pager.in"
#define FRAMES_DEFAULT 3
#define PAGES_DEFAULT 8
#define FRAMESIZE_DEFAULT 512
#define VERBOSE_DEFAULT false
#define MAX_FRAMES 65536

// argErrorChecker function takes in the size of the argument list, the command line argument list array, useDefaultFile flag, the file name, the inputFile, 
// the type, the number of frames, the number of pages, the frame size, the verbose flag, and the flags of each command line argument that requires a specifier 
// after. This function checks all elements of the argument list for errors through the use of other error checking functions defined below. If verbose was 
// inputted on command line, this function also sets verbose flag to true. A call to this function can either cause the program to exit due to an error being 
// found, or the function returns nothing
void argErrorChecker (int argCount, char** argList, bool& useDefaultFile, char* file, ifstream& inFile, char* type, int& frames, int& pages, int& frameSize, bool& typeSpecified, bool& framesSpecified, bool& pagesSpecified, bool& frameSizeSpecified, bool& verbose);

// checkIfArgThatNeedsSpecifier function takes in the the last index of the command line argument list, the index of the current command line argument in the 
// list, the current command line argument, and the command line argument list, the useDefaultFile flag, the type, the number of frames, 
// the number of pages, the frame size, and the flags of each command line argument that requires a specifier after. This function checks if the current command line 
// argument is one of the four that needs a specifier after it. If so, it calls other error checking functions to ensure that this command line argument was 
// inputted properly. This function either causes the program to exit if an error is found, returns true if current command line argument is one of the four 
// that needs a specifier and no errors were found, or returns false if the current command line argument is not one of the four that need specifiers after them
bool checkIfArgThatNeedsSpecifier (int lastArgIndex, int currentArgIndex, const char* currentArg, char** argList, bool& useDefaultFile, char*& type, int& frames, int& pages, int& frameSize, bool& typeSpecified, bool& framesSpecified, bool& pagesSpecified, bool& frameSizeSpecified);

// printAlreadySpecifiedError function takes in the current command line argument. This function prints an error message unique to that command line argument, 
// stating that this command line argument has already been specified before, then exits the program. Returns nothing
void printAlreadySpecifiedError (const char* currentArg);

// argIsLastArg function takes in the current command line argument's index in the command line argument list and the last index of the command line argument
// list. This function checks if the current command line argument's index is equal to the last command line argument's index. Returns true if so, false otherwise
bool argIsLastArg (int lastArgIndex, int currentArgIndex);

// printArgNotSpecifiedError takes in the current command line argument. This function prints an error message unique to the command line argument stating that 
// this command line argument needs a specifier but that one is not provided, then exits the program. Returns nothing
void printArgNotSpecifiedError (const char* arg);

// valueIsFloat function takes in a string. This function checks if the given value is a float or an invalid integer. Returns true if so, false otherwise
bool valueIsFloat (const char* value);

// nextArgIsLastArg function takes in the current command line argument's index in the command line argument list and the last index in the command line 
// argument list. This functionchecks to see if the command line argument after the current command line argument is the last command line argument in 
// the command line argument list. Returns true if so, false otherwise
bool nextArgIsLastArg (int lastArgIndex, int nextArgIndex);

// invalidType function takes in a type string. This function checks if this string is a valid type input. Returns false if it is valid, true if invalid
bool invalidType (string type);

// powerOfTwo function takes in the frame size. This function checks if the given integer for frameSize is a power of two. Returns true if so, false otherwise.
bool powerOfTwo (int frameSize);

// checkIfLooseSpecifierOrTypo function takes in the current command line argument's index of the command line argument list and the command line argument list. 
// This function checks if the current command line argument is a specifier command line argument without its precursor command line argument, or if anything
// is spelled wrong. Returns true if any typos or loose specifiers found, returns false otherwise
bool checkIfLooseSpecifierOrTypo (int currentIndex, char** argList);

// pidChecker function takes in the first string of the input file and an empty string to fill. This function checks if this string is of proper form
// ('P_<pidInteger>'), printing an error message and exiting the program if not. Returns nothing
void pidChecker (string pid, string pidNum);

// printFileFormatError function takes in nothing. This function prints an error message stating that the file is formatted incorrectly, and exits the program. 
// Returns nothing
void printFileFormatError (void);

// ensureAddressIsValid function takes in an address string, a double to be set within the function, the frame size and the number of pages. This function  checks if 
// the address is just an integer, and if this integer is outside the bounds of virtual memory. Returns true if address is an integer and is within the bounds
// of virtual memory, returns false otherwise
void ensureAddressIsValid (string address, double& addressNum, int& frameSize, int& pages);

// stringIsZero function takes in a string. This function checks to see if the string is anything but zero. Returns true if the string is just zero or a bunch
// of 0s, returns false otherwise
bool stringIsZero (string str);

#endif