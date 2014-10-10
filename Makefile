## CS134 Spring 2014: Video Game Design
## Lab 3 Makefile
##
## This file does not need to be modified
#########################################

LIBS = -lglut -lGLU -lGL -lXmu -lXext -lXi -lX11 -lm
CC = g++

## Global header files
INCLUDE = const.h color.h

## Object files and executables
MAIN_OUT = assn2
VECTOR3_OUT = vector3.o object.o
PARTICLESYSTEM_OUT = particlesystem.o

## Requirements for each command
MAIN_REQS = main.cpp $(VECTOR3_OUT) $(PARTICLESYSTEM_OUT)
VECTOR3_REQS = vector3.h vector3.cpp
PARTICLESYSTEM_REQS = particlesystem.h particlesystem.cpp


## Targets to compile for each command
MAIN_TARGETS = main.cpp $(VECTOR3_OUT) $(PARTICLESYSTEM_OUT)
VECTOR3_TARGETS = vector3.cpp object.cpp
PARTICLESYSTEM_TARGETS = particlesystem.cpp

all: main

## Main 
main: $(MAIN_REQS) $(INCLUDE)
	$(CC) $(MAIN_TARGETS) $(LIBS) -o $(MAIN_OUT)

## Vector3 class
$(VECTOR3_OUT): $(VECTOR3_REQS) $(INCLUDE)
	$(CC) -c $(VECTOR3_TARGETS)

## Particle System class
$(PARTICLESYSTEM_OUT): $(PARTICLESYSTEM_REQS) $(INCLUDE)
	$(CC) -c $(PARTICLESYSTEM_TARGETS)

clean:
	rm -f *~ *.o *.out *assn2

