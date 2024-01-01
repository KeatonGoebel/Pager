# Pager
Pager is a C++ implementation of a paging simulation program. I wrote it in the fall semester of my senior year at Transylvania University. It was also a group project written with two other people. Paging is a technique where the operating system separates processes into pages and physical memory into fixed-size blocks called frames. When the operating system executes a process, it maps the pages representing the process onto one or more frames representing physical memory. The operating system keeps track of what pages correspond to what frames with a page table. This technique allows the operating system to translate logical memory addresses into physical memory. Paging allows for there to be a noncontiguous allocation of physical memory. Because the frames can be stored anywhere in memory, memory can be used much more efficiently. 
## How Does Pager Work?
Pager takes in a list of arguments from the command line to allow the user to make specifications about the simulation. The allowable arguments are "--type," "--pages," "--framesize," "--frames," and "--verbose." After the argument "--type," the user can also input FIFO, LRU, MFU, or Random. These keywords tell the program what paging algorithm they will use to allocate frames. They represent first come first serve, least recently used, most frequently used, and random. After the arguments "--frames," "--pages," and "--framesize," the user can input an integer to represent their desired values. However, for framesize, the inputted integer must be a power of two. Inputting the argument "--verbose" shows the process of the page table throughout the algorithm. The user can also input files of proper form at the end of the command line. If no argument is inputted, then each argument will assume a default value. The type will default to FIFO, frames will default to 3, pages will default to 8, frame size will default to 512, verbose will default to false, and the input file will default to pager.in. 
### Pager Example Inputs and Outputs 
Pager is designed to read input files following a proper form. The input file will always have one line meant to specify the process ID followed by lines of memory addresses. The process ID must start with the characters "P_" followed by an integer. The memory addresses will always be represented by integers. I have also included two input files for testing purposes: pager.in, and pager2.in. 

Input File:
```
P_001
2001
30
3576
2001
3517
891
3576
3576
2001
3576
3576
2001
```
Input From User: 
```
./pager --type FIFO --pages 7 --frames 4
```
Output:
```
***** P_001 finished executing with a total of 4 page faults! *****
```
