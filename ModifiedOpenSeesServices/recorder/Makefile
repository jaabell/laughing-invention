# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/..

include ../../Makefile.Includes
include ../../Makefile.Compilers

OBJS       = Recorder.o \
             DatastoreRecorder.o \
             MaxNodeDispRecorder.o \
             ElementRecorder.o \
             NodeRecorder.o \
             DriftRecorder.o \
             EnvelopeNodeRecorder.o \
             EnvelopeElementRecorder.o \
             PatternRecorder.o
#             GSA_Recorder.o \
#              DamageRecorder.o
#             TclRecorderCommands.o \
#              YsVisual.o \
#              FilePlotter.o \
#              AlgorithmIncrements.o \


# Compilation control

all:         $(OBJS)
	@$(CD) response; $(MAKE);

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean
	@$(CD) response; $(MAKE) wipe;

wipe: spotless


# DO NOT DELETE THIS LINE -- make depend depends on it.
