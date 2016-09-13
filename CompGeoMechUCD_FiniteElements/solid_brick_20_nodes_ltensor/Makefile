

include ../../Makefile.Includes
include ../../Makefile.Compilers




################### TARGETS ########################
all: TwentyNodeBrickLT.o   
	$(AR) $(ARFLAGS) ../../lib/libsolid_brick_20_nodes_ltensor.a  TwentyNodeBrickLT.o  


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libsolid_brick_20_nodes_ltensor.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


