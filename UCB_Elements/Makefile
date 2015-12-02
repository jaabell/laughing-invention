# updated by:
# Nima Tafazzoli, June 2012, UCD


include ../Makefile.Compilers
include ../Makefile.Includes


LIBRARY_UCB_Elements  = ../lib/libucbelements.a


UCB_ELEMENTS =  ./dispBeamColumn/DispBeamColumn3d.o \
                ./elasticBeamLumpedMass/ElasticBeamLumpedMass.o \
                ./joint/BeamColumnJoint3d.o \
                ./joint/Joint3D.o \
                ./joint/MP_Joint3D.o \
                ./shell/ShellMITC4.o \
                ./shell/NewShellMITC4.o \
                ./shell/R3vectors.o \
                ./truss/CorotTruss.o \
                ./truss/CorotTrussSection.o \
                ./truss/Truss.o \
                ./truss/TrussSection.o \
                ./zeroLength/ZeroLength.o \
                ./integration/BeamIntegration.o \
                ./integration/HingeMidpointBeamIntegration3d.o \
                ./integration/HingeRadauBeamIntegration3d.o \
                ./integration/HingeRadauTwoBeamIntegration3d.o \
                ./integration/LegendreBeamIntegration.o \
                ./integration/LobattoBeamIntegration.o \
                ./integration/NewtonCotesBeamIntegration.o \
                ./integration/RadauBeamIntegration.o \
                ./beamWithHinges/BeamWithHinges3d.o




all:    
	@$(CD) truss; $(MAKE);
	@$(CD) zeroLength; $(MAKE);
	@$(CD) beamWithHinges; $(MAKE);
	@$(CD) shell; $(MAKE);
	@$(CD) joint; $(MAKE);
	@$(CD) dispBeamColumn; $(MAKE);
	@$(CD) elasticBeamLumpedMass; $(MAKE);
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
	@$(CD) integration; $(MAKE) clean;
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS) $(LIBRARY_UCB_Elements)

