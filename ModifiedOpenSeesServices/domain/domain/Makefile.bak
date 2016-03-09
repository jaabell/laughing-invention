# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers


OBJS       = Domain.o

# Compilation control

all:         $(OBJS)
	     @$(CD) single; $(MAKE);
ifeq ($(PROGRAMMING_MODE), PARALLEL)
	     @$(CD) partitioned; $(MAKE);
endif

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(CD) single; $(MAKE) wipe clean;
	@$(CD) partitioned; $(MAKE) wipe clean;
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless
	@$(CD) single; $(MAKE) wipe;
	@$(CD) partitioned; $(MAKE) wipe;

# DO NOT DELETE THIS LINE -- make depend depends on it.
