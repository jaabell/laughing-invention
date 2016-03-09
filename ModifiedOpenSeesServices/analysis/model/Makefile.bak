# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers


OBJS       = AnalysisModel.o 

# Compilation control

all:         $(OBJS)
	@$(CD) simple; $(MAKE); 

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o
	@$(CD) simple; $(MAKE) wipe

cleanall: clean

wipe: clean


# DO NOT DELETE THIS LINE -- make depend depends on it.
