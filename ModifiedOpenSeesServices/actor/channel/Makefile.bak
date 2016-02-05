current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers

ifeq ($(PROGRAMMING_MODE), PARALLEL)
OBJS	=	MPI_Channel.o Channel.o
else
OBJS	=	Channel.o
endif

all:       $(OBJS)

mpi: MPI_Channel.o

tcp: TCP_Socket.o UDP_Socket.o

tidy:
	@$(RM) $(RMFLAGS) Makefile.bak *~ 

clean:  tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless

.PHONY: test_hdf5_channel
test_hdf5_channel: test_hdf5_channel.o
	$(LINKER) $(LINKFLAGS) \
	$(OBJS) test_hdf5_channel.o \
	$(LIBS) \
	-o $@ 
