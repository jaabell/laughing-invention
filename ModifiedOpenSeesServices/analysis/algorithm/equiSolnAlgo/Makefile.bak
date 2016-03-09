# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../../..

include ../../../../Makefile.Includes
include ../../../../Makefile.Compilers


OBJS       = EquiSolnAlgo.o \
             Linear.o \
             NewtonRaphson.o \
             ModifiedNewton.o
#              NewtonLineSearch.o \
#              Broyden.o \
#              BFGS.o \
#              KrylovNewton.o \
#              PeriodicNewton.o \
#              LineSearch.o \
#              InitialInterpolatedLineSearch.o \
#              SecantLineSearch.o \
#              RegulaFalsiLineSearch.o \
#              BisectionLineSearch.o

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
