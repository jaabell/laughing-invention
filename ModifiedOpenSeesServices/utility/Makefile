# updated by:
# Nima Tafazzoli, November 2009, UCD
#updated by Babak in September 2012

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/..

include ../../Makefile.Compilers
include ../../Makefile.Includes


OBJS       = NanoTimer.o Timer.o 

# Compilation control

all:         $(OBJS) 

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.
