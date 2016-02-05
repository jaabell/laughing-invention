# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/..

include ../../Makefile.Includes
include ../../Makefile.Compilers

OBJS       = Material.o

all:         $(OBJS)
	@$(CD) uniaxial; $(MAKE);
# 	@$(CD) nD; $(MAKE);
	@$(CD) section; $(MAKE);
#	@$(CD) yieldSurface; $(MAKE);

# Miscellaneous

tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean
	@$(CD) uniaxial; $(MAKE) wipe;
# 	@$(CD) nD; $(MAKE) wipe;
	@$(CD) section; $(MAKE) wipe;
	@$(CD) yieldSurface; $(MAKE) wipe;

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.
