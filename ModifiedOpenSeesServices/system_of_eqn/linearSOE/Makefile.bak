# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers

OBJS       = LinearSOE.o \
             DomainSolver.o \
             LinearSOESolver.o

all:         $(OBJS)
ifeq ($(PROGRAMMING_MODE), SEQUENTIAL)
	@$(CD) bandSPD; $(MAKE);
	@$(CD) bandGEN; $(MAKE);
	@$(CD) profileSPD; $(MAKE);
	@$(CD) umfGEN; $(MAKE);
endif
ifeq ($(PROGRAMMING_MODE), PARALLEL)
	@$(CD) petsc; $(MAKE);
endif


# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean
	@$(CD) bandSPD; $(MAKE) wipe;
	@$(CD) bandGEN; $(MAKE) wipe;
	@$(CD) fullGEN; $(MAKE) wipe;
	@$(CD) profileSPD; $(MAKE) wipe;
	@$(CD) sparseGEN; $(MAKE) wipe;
	@$(CD) sparseSYM; $(MAKE) wipe;
	@$(CD) umfGEN; $(MAKE) wipe;
	@$(CD) cg; $(MAKE) wipe;
	@$(CD) BJsolvers; $(MAKE) wipe;
	@$(CD) diagonal; $(MAKE) wipe;
	@$(CD) petsc; $(MAKE) wipe;

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.
