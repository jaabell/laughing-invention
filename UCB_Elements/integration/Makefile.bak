# updated by:
# Nima Tafazzoli, June 2012, UCD


include ../../Makefile.Includes
include ../../Makefile.Compilers


OBJS       = BeamIntegration.o \
             LobattoBeamIntegration.o \
             LegendreBeamIntegration.o \
             RadauBeamIntegration.o \
             NewtonCotesBeamIntegration.o \
             HingeMidpointBeamIntegration3d.o \
             HingeRadauBeamIntegration3d.o \
             HingeRadauTwoBeamIntegration3d.o
#              UserDefinedHingeIntegration3d.o
#              UserDefinedBeamIntegration.o \
             
################### TARGETS ########################
all:         $(OBJS)

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.

