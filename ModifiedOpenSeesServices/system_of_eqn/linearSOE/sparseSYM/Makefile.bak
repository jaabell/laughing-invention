# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../../..

include ../../../../Makefile.Includes
include ../../../../Makefile.Compilers

PROGRAM         = test

OBJS       =  SymSparseLinSOE.o \
              SymSparseLinSolver.o

all:         $(OBJS) law

law:   grcm.o      nest.o   nmat.o   symbolic.o  utility.o \
       newordr.o   nnsim.o  genmmd.o

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean
	@$(RM) $(RMFLAGS) $(PROGRAM) 

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.
