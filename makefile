##
# PROGRAM:    Assignment 3
# PROGRAMMER: Tyler Saballus
# LOGON ID:   z1836409
# DATE DUE:   09-29-2018
#

# Compiler variables
CC = g++
CCFLAGS = -Wall

# Rule to link object code files to create executable file
Assign3: hw3.o
	$(CC) $(CCFLAGS) -o Assign3 hw3.o

# Rules to compile source code file to object code
hw3.o: hw3.cc
	$(CC) $(CCFLAGS) -c hw3.cc

# Pseudo-target to remove object code and executable files
clean:
	-rm *.o Assign3
