# FILE: makefile
# J Hall, K Goebel, M Baird, Transy University
# Fall 2023
#
#	Build's an executable pager file by ensuring .o and output files are recompiled if source code is changed.
#
# target: pager's executable file [pager]
pager: pager.o pageTable.o pageReplacementSimulator.o
	g++ pager.o pageTable.o pageReplacementSimulator.o -o pager

# target: pager's object file [pager.o]
pager.o: pager.cpp pager.h pageTable.h pageReplacementSimulator.h
	g++ -c pager.cpp 

# target: pageTable's object file [pageTable.o]
pageTable.o: pageTable.cpp pageTable.h
	g++ -c pageTable.cpp 
	
# target: pageReplacementSimulator's object file [pageReplacementSimulator.o]
pageReplacementSimulator.o: pageReplacementSimulator.cpp pageReplacementSimulator.h
	g++ -c pageReplacementSimulator.cpp 
	