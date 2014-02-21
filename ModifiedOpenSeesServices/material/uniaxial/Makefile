# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..


include ../../../Makefile.Includes
include ../../../Makefile.Compilers

 OBJS       = UniaxialMaterial.o \
              ElasticMaterial.o \
              ElasticPPMaterial.o \
              ParallelMaterial.o \
              HardeningMaterial.o \
              Concrete01.o \
              Concrete04.o \
              Steel01.o \
              HystereticMaterial.o \
              EPPGapMaterial.o \
              ViscousMaterial.o \
              SeriesMaterial.o \
              MinMaxMaterial.o \
              PathIndependentMaterial.o \
              CableMaterial.o \
              ENTMaterial.o \
              BoucWenMaterial.o \
              BarSlipMaterial.o \
              FatigueMaterial.o \
              Steel02.o \
              Concrete02.o \
              Steel03.o \
              KinematicHardening.o \
              NewUniaxialMaterial.o \
              ReinforcingSteel.o
#              Pinching4Material.o \

all:         $(OBJS)


# Miscellaneous

tidy:
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean


wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.
