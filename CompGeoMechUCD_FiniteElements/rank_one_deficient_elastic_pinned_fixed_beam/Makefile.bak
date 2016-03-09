# Nima Tafazzoli, December 2012, UCD

include ../../Makefile.Includes
include ../../Makefile.Compilers


################### TARGETS ########################
all: rank_one_deficient_elastic_pinned_fixed_beam.o
	$(AR) $(ARFLAGS) ../../lib/librank_one_deficient_elastic_pinned_fixed_beam.a  rank_one_deficient_elastic_pinned_fixed_beam.o


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/librank_one_deficient_elastic_pinned_fixed_beam.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


