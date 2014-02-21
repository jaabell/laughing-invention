# Nima Tafazzoli, December 2012, UCD


include ../../Makefile.Includes
include ../../Makefile.Compilers


################### TARGETS ########################
all: ElasticBeam.o
	$(AR) $(ARFLAGS) ../../lib/libelasticbeam.a  ElasticBeam.o


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libelasticbeam.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


