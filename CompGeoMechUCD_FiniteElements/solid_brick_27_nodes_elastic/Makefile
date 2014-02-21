#Boris Jeremic 12April2009

include ../../Makefile.Includes
include ../../Makefile.Compilers


################### TARGETS ########################
all: TwentySevenNodeBrickElastic.o   
	$(AR) $(ARFLAGS) ../../lib/libsolid_brick_27_nodes_elastic.a  TwentySevenNodeBrickElastic.o  


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libsolid_brick_27_nodes_elastic.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


