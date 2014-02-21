# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers

OBJS       = EigenSOE.o \
             EigenSolver.o \
             BandArpackSOE.o \
             BandArpackSolver.o \
             SymArpackSOE.o \
             SymArpackSolver.o \
             SymBandEigenSOE.o \
             SymBandEigenSolver.o \
             FullGenEigenSOE.o \
             FullGenEigenSolver.o

all:    $(OBJS)

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean
	@$(RM) $(RMFLAGS)

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.
