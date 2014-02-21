# updated by:
# Nima Tafazzoli, October 2010, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers

OBJS       = Damping.o \
             Rayleigh.o \
             Caughey3rd.o \
             Caughey4th.o

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
