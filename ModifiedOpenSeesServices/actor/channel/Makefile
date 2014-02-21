# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers


#OBJS	=	Channel.o TCP_Socket.o UDP_Socket.o 

# Added by B. Kamrani for parallel, Jan. 2011
ifeq ($(PROGRAMMING_MODE), PARALLEL)

OBJS	=	MPI_Channel.o Channel.o

	
else
OBJS	=	Channel.o 
endif
#
# Out by B. Kamrani for parallel, Jan. 2011

#OBJS	=	Channel.o 

# MPI_Channel.o

# Compilation control

all:       $(OBJS)

mpi: MPI_Channel.o

tcp: TCP_Socket.o UDP_Socket.o

# Miscellaneous
tidy:
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean:  tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless


# DO NOT DELETE THIS LINE -- make depend depends on it.



