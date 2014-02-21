# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/..

include ../../Makefile.Includes
include ../../Makefile.Compilers


all:        
	     @$(CD) analysis; $(MAKE);
	     @$(CD) integrator; $(MAKE);
	     @$(CD) model; $(MAKE);
	     @$(CD) numberer; $(MAKE);
	     @$(CD) handler; $(MAKE);
	     @$(CD) algorithm; $(MAKE);
	     @$(CD) dof_grp; $(MAKE);
	     @$(CD) fe_ele; $(MAKE);

# Miscellaneous
tidy:	
	@rm -f Makefile.bak *~ #*# core

clean: tidy
	@rm -f $(OBJS) *.o

spotless: clean

wipe: spotless
	     @$(CD) analysis; $(MAKE) wipe;
	     @$(CD) integrator; $(MAKE) wipe;
	     @$(CD) model; $(MAKE) wipe;
	     @$(CD) numberer; $(MAKE) wipe;
	     @$(CD) handler; $(MAKE) wipe;
	     @$(CD) algorithm; $(MAKE) wipe;
	     @$(CD) dof_grp; $(MAKE) wipe;
	     @$(CD) fe_ele; $(MAKE) wipe;


# DO NOT DELETE THIS LINE -- make depend depends on it.
