# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers


OBJS       = NDMaterial.o \
             PlaneStressMaterial.o \
             BeamFiberMaterial.o \
             ElasticIsotropicBeamFiber.o \
             ElasticIsotropicMaterial.o 


all: $(OBJS) 


# Miscellaneous

tidy:
	$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	$(RM) $(RMFLAGS) $(OBJS) *.o
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map

spotless: clean

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.



