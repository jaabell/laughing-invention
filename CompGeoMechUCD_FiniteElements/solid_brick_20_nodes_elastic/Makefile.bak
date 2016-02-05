#Boris Jeremic 12April2009

include ../../Makefile.Includes
include ../../Makefile.Compilers


################### TARGETS ########################
all: TwentyNodeBrickElastic.o   
	$(AR) $(ARFLAGS) ../../lib/libsolid_brick_20_nodes_elastic.a  TwentyNodeBrickElastic.o  


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libsolid_brick_20_nodes_elastic.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


