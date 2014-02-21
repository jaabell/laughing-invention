#Boris Jeremic 12April2009

include ../../Makefile.Includes
include ../../Makefile.Compilers


################### TARGETS ########################
all: PenaltyElementApplyDisplacement.o
	$(AR) $(ARFLAGS) ../../lib/libpenaltyelementapplydisplacement.a  PenaltyElementApplyDisplacement.o


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libpenaltyelementapplydisplacement.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


