# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/..

include ../../Makefile.Includes
include ../../Makefile.Compilers

OBJS       = 

# Compilation control

all:         $(OBJS)
	@$(CD) graph; $(MAKE);
	@$(CD) numberer; $(MAKE);
ifeq ($(PROGRAMMING_MODE), PARALLEL)
	@$(CD) partitioner; $(MAKE);
endif

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core 

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless
	@$(CD) graph; $(MAKE) wipe;
	@$(CD) numberer; $(MAKE) wipe;
	@$(CD) partitioner; $(MAKE) wipe;


# DO NOT DELETE THIS LINE -- make depend depends on it.


