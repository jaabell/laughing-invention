# updated by:
# Nima Tafazzoli, November 2009, UCD
# Nima Tafazzoli, adding damping folder, October 2010, UCD


current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/..

include ../../Makefile.Includes
include ../../Makefile.Compilers

OBJS       = 

#	@$(CD) $(FE)/domain/partitioner; $(MAKE);
#	@$(CD) $(FE)/domain/loadBalancer; $(MAKE);

all:     
	@$(CD) domain; $(MAKE);
	@$(CD) component; $(MAKE);
	@$(CD) load; $(MAKE);
	@$(CD) node; $(MAKE);
	@$(CD) constraints; $(MAKE);
	@$(CD) pattern; $(MAKE);
	@$(CD) groundMotion; $(MAKE);
	@$(CD) damping; $(MAKE);
ifeq ($(PROGRAMMING_MODE), PARALLEL)
	@$(CD) subdomain; $(MAKE);
	@$(CD) partitioner; $(MAKE);
endif
# 	@$(CD) region; $(MAKE);

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy

spotless: clean

wipe: spotless
	@$(CD) domain; $(MAKE) wipe;
	@$(CD) component; $(MAKE) wipe;
	@$(CD) load; $(MAKE) wipe;
	@$(CD) subdomain; $(MAKE) wipe;
	@$(CD) node; $(MAKE) wipe;
	@$(CD) constraints; $(MAKE) wipe;
	@$(CD) partitioner; $(MAKE) wipe;
	@$(CD) loadBalancer; $(MAKE) wipe;
	@$(CD) pattern; $(MAKE) wipe;
	@$(CD) groundMotion; $(MAKE) wipe;
	@$(CD) partitioner; $(MAKE) wipe;
	@$(CD) damping; $(MAKE) wipe;

	# @$(CD) region; $(MAKE) wipe;
# DO NOT DELETE THIS LINE -- make depend depends on it.
