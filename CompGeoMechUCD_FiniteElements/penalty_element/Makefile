#Boris Jeremic 12April2009


include ../../Makefile.Includes
include ../../Makefile.Compilers


################### TARGETS ########################
all: PenaltyElement.o
	$(AR) $(ARFLAGS) ../../lib/libpenaltyelement.a  PenaltyElement.o


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libpenaltyelement.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


