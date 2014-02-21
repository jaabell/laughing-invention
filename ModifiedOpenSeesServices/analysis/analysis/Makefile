# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers


OBJS       = DomainUser.o \
             Analysis.o \
             StaticAnalysis.o \
             TransientAnalysis.o \
             DirectIntegrationAnalysis.o \
             DomainDecompositionAnalysis.o \
             SubstructuringAnalysis.o \
             EigenAnalysis.o \
             VariableTimeStepDirectIntegrationAnalysis.o \
             StaticDomainDecompositionAnalysis.o \
             TransientDomainDecompositionAnalysis.o

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
