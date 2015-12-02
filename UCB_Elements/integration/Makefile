# updated by:
# Nima Tafazzoli, June 2012, UCD


include ../../Makefile.Includes
include ../../Makefile.Compilers


OBJS       = BeamIntegration.o \
		LegendreBeamIntegration.o \
		UserDefinedBeamIntegration.o \
		HingeMidpointBeamIntegration3d.o \
		LobattoBeamIntegration.o \
		UserDefinedHingeIntegration3d.o \
		HingeRadauBeamIntegration3d.o \
		NewtonCotesBeamIntegration.o \
		HingeRadauTwoBeamIntegration3d.o \
		RadauBeamIntegration.o 

             
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

