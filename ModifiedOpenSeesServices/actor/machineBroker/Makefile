# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers

OBJS = MachineBroker.o MPI_MachineBroker.o

# OBJS = MachineBroker.o \
#        AlphaMachineBroker.o \
#        DecMachineBroker.o \
#        MillMachineBroker.o \
#        MPI_MachineBroker.o

all: $(OBJS)

mpi: MPI_MachineBroker.o

gexec: GEXEC_MachineBroker.o

# Miscellaneous
tidy:
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean:  tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean
	@$(RM) $(RMFLAGS) $(PROGRAM) fake

wipe: spotless


# DO NOT DELETE THIS LINE -- make depend depends on it.



