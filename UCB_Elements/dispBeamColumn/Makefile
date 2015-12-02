# updated by:
# Nima Tafazzoli, June 2012, UCD


include ../../Makefile.Includes
include ../../Makefile.Compilers


OBJS       = BeamIntegration.o \
	FiberSection3d.o \
	LinearCrdTransf3d.o \
	Concrete01.o \
	LobattoBeamIntegration.o \
	CorotCrdTransf3d.o \
	CrdTransf.o \
	PDeltaCrdTransf3d.o \
	DispBeamColumn3d.o \
	SectionForceDeformation.o \
	Steel01.o \
	ElasticMaterial.o \
	UniaxialFiber3d.o \
	ElasticSection3d.o \
	UniaxialMaterial.o \
	Fiber.o


             
################### TARGETS ########################
all:         $(OBJS)


test: $(UCB_ELEMENTS) CantileverPureBending.o 
	$(CC++) CantileverPureBending.o DispBeamColumn3d.o CorotCrdTransf3d.o CrdTransf.o $(LIBS) -o CantileverPureBending 

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.

