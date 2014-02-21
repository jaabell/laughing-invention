# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../../..


include ../../../../Makefile.Includes
include ../../../../Makefile.Compilers

OBJS       = LimitStateMaterial.o \
#             TclLimitState.o


all:         $(OBJS)
	@$(CD) limitCurve; $(MAKE);

# Miscellaneous

tidy:   
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean
	@$(CD) limitCurve; $(MAKE) wipe;

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.
