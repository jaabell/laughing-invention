# Makefile for fe objects

# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/..

include ../../Makefile.Includes
include ../../Makefile.Compilers

OBJS       = SystemOfEqn.o Solver.o

all: $(OBJS)
	@$(CD) linearSOE; $(MAKE);
	@$(CD) eigenSOE; $(MAKE);

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean
	@$(CD) linearSOE; $(MAKE) wipe;
	@$(CD) eigenSOE; $(MAKE) wipe;

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.
