# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..


include ../../../Makefile.Includes
include ../../../Makefile.Compilers


all:
	@$(CD) yieldSurfaceBC; $(MAKE);
	@$(CD) plasticHardeningMaterial; $(MAKE);
	@$(CD) evolution; $(MAKE);

# Miscellaneous

tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean
	@$(CD) yieldSurfaceBC; $(MAKE) wipe;
	@$(CD) plasticHardeningMaterial; $(MAKE) wipe;
	@$(CD) evolution; $(MAKE) wipe;

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.
