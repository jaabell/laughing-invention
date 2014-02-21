# Makefile for fe objects

# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../../..

include ../../../../Makefile.Includes
include ../../../../Makefile.Compilers

PROGRAM = test

OBJS       = ProfileSPDLinSOE.o \
             ProfileSPDLinSolver.o \
             ProfileSPDLinDirectSolver.o \
             ProfileSPDLinSubstrSolver.o \
             ProfileSPDLinDirectBlockSolver.o \
             ProfileSPDLinDirectSkypackSolver.o \
             ProfileSPDLinSolverGather.o \
             ProfileSPDLinSOEGather.o \
             DistributedProfileSPDLinSOE.o

all:         $(OBJS)

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean
	@$(RM) $(RMFLAGS) $(PROGRAM) 

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.
