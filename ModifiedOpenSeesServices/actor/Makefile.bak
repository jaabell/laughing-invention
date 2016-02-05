# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/..

include ../../Makefile.Includes
include ../../Makefile.Compilers

HOST            = Host
NODE		= Node

PROGRAM         = $(Host) $(Node)

HOST_OBJS       = Host.o 
NODE_OBJS       = Node.o 

# Compilation control

PROGRAM = /tmp/go

OBJS = 

all:        
ifeq ($(PROGRAMMING_MODE), PARALLEL)
	@$(CD) channel; $(MAKE);
	@$(CD) machineBroker; $(MAKE);
	@$(CD) message; $(MAKE);
	@$(CD) address; $(MAKE);
	@$(CD) actor; $(MAKE);
	@$(CD) shadow; $(MAKE);	
	@$(CD) objectBroker; $(MAKE);
else
	@$(CD) message; $(MAKE);
	@$(CD) channel; $(MAKE);
	@$(CD) actor; $(MAKE);
	@$(CD) objectBroker; $(MAKE);
endif

$(HOST):     $(HOST_OBJS) 
	     @echo "Linking $(PROGRAM)..."
	     $(LINKER) $(LINKFLAGS) $(HOST_OBJS) $(FE_LIBRARY) \
	     $(MACHINE_LINKLIBS) $(BLAS_LIBS) \
	     $(MACHINE_NUMERICAL_LIBS) $(MACHINE_SPECIFIC_LIBS) \
	     -o $(HOST)

$(NODE):     $(NODE_OBJS) 
	     @echo "Linking $(PROGRAM)..."
	     $(LINKER) $(LINKFLAGS) $(NODE_OBJS) $(FE_LIBRARY) \
	     $(MACHINE_LINKLIBS) $(BLAS_LIBS) \
	     $(MACHINE_NUMERICAL_LIBS) $(MACHINE_SPECIFIC_LIBS) \
	     -o $(NODE)

tidy:
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean:  tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean
	@$(RM) $(RMFLAGS) fake $(HOST) $(NODE)

wipe: spotless
	@$(CD) channel; $(MAKE) wipe;
	@$(CD) objectBroker; $(MAKE) wipe;
	@$(CD) machineBroker; $(MAKE) wipe;
	@$(CD) message; $(MAKE) wipe;
	@$(CD) address; $(MAKE) wipe;
	@$(CD) shadow; $(MAKE) wipe;
	@$(CD) actor; $(MAKE) wipe;

# DO NOT DELETE THIS LINE -- make depend depends on it.



