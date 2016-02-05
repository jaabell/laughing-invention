# Makefile for element objects

# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers

OBJS       = StaticIntegrator.o \
             TransientIntegrator.o \
             HHT.o \
             HHT1.o \
             HHT_alpha_method.o \
             WilsonTheta.o \
             Newmark.o \
             CentralDifference.o \
             Integrator.o \
             LoadControl.o \
             ArcLength.o \
             ArcLength1.o \
             LoadPath.o \
             DisplacementControl.o \
             MinUnbalDispNorm.o \
             EigenIntegrator.o \
             HSConstraint.o \
             DistributedDisplacementControl.o \
             CentralDifferenceAlternative.o \
             CentralDifferenceNoDamping.o \
			 IncrementalIntegrator.o 
#             Newmark1.o \
#             HHT1.o \

all:         $(OBJS)

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless


# DO NOT DELETE THIS LINE -- make depend depends on it.
