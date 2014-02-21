# updated by:
# Nima Tafazzoli, November 2009, UCD

# Makefile for fe objects

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../../..

include ../../../../Makefile.Includes
include ../../../../Makefile.Compilers

PROGRAM = test

OBJS       = skymatr.o  \
             profmatr.o


################### TARGETS ########################
all: $(OBJS) 


libBJsolvers.a: $(OBJS)
	$(AR) -rv $(HOME)/lib/libBJsolvers.a $(OBJS)

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean
	@$(RM) $(RMFLAGS) $(PROGRAM) 

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.
