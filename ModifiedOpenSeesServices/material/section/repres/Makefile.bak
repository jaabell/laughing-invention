# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../../..

include ../../../../Makefile.Includes
include ../../../../Makefile.Compilers

OBJS       =

# Compilation control

all:	
	@$(CD) cell; $(MAKE);
	@$(CD) patch; $(MAKE);
	@$(CD) reinfBar; $(MAKE);
	@$(CD) reinfLayer; $(MAKE);
	@$(CD) section; $(MAKE);

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy

spotless: clean

wipe: spotless
	@$(CD) cell; $(MAKE) wipe;
	@$(CD) patch; $(MAKE) wipe;
	@$(CD) reinfBar; $(MAKE) wipe;
	@$(CD) reinfLayer; $(MAKE) wipe;
	@$(CD) section; $(MAKE) wipe;

# DO NOT DELETE THIS LINE -- make depend depends on it.
