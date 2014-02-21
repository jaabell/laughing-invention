# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers


OBJS       = FE_Element.o

# Compilation control

all:         $(OBJS)
	@$(CD) penalty; $(MAKE);
	@$(CD) lagrange; $(MAKE);
	@$(CD) transformation; $(MAKE);

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless
	@$(CD) penalty; $(MAKE) wipe;
	@$(CD) lagrange; $(MAKE) wipe;
	@$(CD) transformation; $(MAKE) wipe;


# DO NOT DELETE THIS LINE -- make depend depends on it.
