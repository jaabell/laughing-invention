# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers

OBJS       = SolutionAlgorithm.o

# Compilation control

all:         $(OBJS)
	     @$(CD) domainDecompAlgo; $(MAKE);
	     @$(CD) equiSolnAlgo; $(MAKE);
	     @$(CD) eigenAlgo; $(MAKE);

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless
	     @$(CD) domainDecompAlgo; $(MAKE) wipe;
	     @$(CD) equiSolnAlgo; $(MAKE) wipe;
	     @$(CD) eigenAlgo; $(MAKE) wipe;

# DO NOT DELETE THIS LINE -- make depend depends on it.






