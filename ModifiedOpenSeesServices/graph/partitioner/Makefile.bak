# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers

OBJS       = Metis.o \
             ParMetis.o

# Compilation control

all: $(OBJS)

#ParMetis.o:
#	@$(ECHO) Making $@ from $<
#	mpicc $(C++FLAGS) $(INCLUDES) -c $< -o $@
#
#mpi: ParMetis.o


# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.


