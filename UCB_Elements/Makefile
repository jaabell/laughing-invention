# updated by:
# Nima Tafazzoli, June 2012, UCD


include ../Makefile.Compilers
include ../Makefile.Includes


LIBRARY_UCB_Elements  = ../lib/libucbelements.a


UCB_ELEMENTS =  ./shell/R3vectors.o \
		./shell/NewShellMITC4.o \
		./shell/ShellMITC4.o \
		./dispBeamColumn/DispBeamColumn3d.o \
		./transformation/PDeltaCrdTransf3d.o \
		./transformation/CorotCrdTransf3d.o \
		./transformation/LinearCrdTransf3d.o \
		./transformation/CrdTransf.o \
		./truss/TrussSection.o \
		./truss/CorotTrussSection.o \
		./truss/CorotTruss.o \
		./truss/Truss.o \
		./elasticBeamLumpedMass/ElasticBeamLumpedMass.o \
		./integration/BeamIntegration.o \
		./integration/HingeMidpointBeamIntegration3d.o \
		./integration/LobattoBeamIntegration.o \
		./integration/UserDefinedBeamIntegration.o \
		./integration/LegendreBeamIntegration.o \
		./integration/HingeRadauBeamIntegration3d.o \
		./integration/RadauBeamIntegration.o \
		./integration/HingeRadauTwoBeamIntegration3d.o \
		./integration/NewtonCotesBeamIntegration.o \
		./integration/UserDefinedHingeIntegration3d.o \
		./zeroLength/ZeroLength.o \
		./joint/BeamColumnJoint3d.o \
		./joint/MP_Joint3D.o \
		./joint/Joint3D.o \
		./beamWithHinges/BeamWithHinges3d.o 





all:    
	@$(CD) truss; $(MAKE);
	@$(CD) zeroLength; $(MAKE);
	@$(CD) beamWithHinges; $(MAKE);
	@$(CD) shell; $(MAKE);
	@$(CD) joint; $(MAKE);
	@$(CD) dispBeamColumn; $(MAKE);
	@$(CD) elasticBeamLumpedMass; $(MAKE);
	@$(CD) transformation; $(MAKE);
	@$(CD) integration; $(MAKE);
	$(AR) $(ARFLAGS) $(LIBRARY_UCB_Elements) $(UCB_ELEMENTS)



clean:    
	@$(CD) truss; $(MAKE) clean;
	@$(CD) zeroLength; $(MAKE) clean;
	@$(CD) beamWithHinges; $(MAKE) clean;
	@$(CD) shell; $(MAKE) clean;
	@$(CD) joint; $(MAKE) clean;
	@$(CD) dispBeamColumn; $(MAKE) clean;
	@$(CD) elasticBeamLumpedMass; $(MAKE) clean;
	@$(CD) transformation; $(MAKE) clean;
	@$(CD) integration; $(MAKE) clean;
	@$(RM) $(RMFLAGS) $(OBJS) *.o


cleanall:    
	@$(CD) truss; $(MAKE) clean;
	@$(CD) zeroLength; $(MAKE) clean;
	@$(CD) beamWithHinges; $(MAKE) clean;
	@$(CD) shell; $(MAKE) clean;
	@$(CD) joint; $(MAKE) clean;
	@$(CD) dispBeamColumn; $(MAKE) clean;
	@$(CD) elasticBeamLumpedMass; $(MAKE) clean;
	@$(CD) transformation; $(MAKE) clean;
	@$(CD) integration; $(MAKE) clean;
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS) $(LIBRARY_UCB_Elements)

