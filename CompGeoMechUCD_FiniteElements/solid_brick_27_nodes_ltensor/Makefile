#Jose Abell November 4, 2013


include ../../Makefile.Includes
include ../../Makefile.Compilers


################### TARGETS ########################
all: TwentySevenNodeBrickLT.o   
	$(AR) $(ARFLAGS) ../../lib/libsolid_brick_27_nodes_ltensor.a  TwentySevenNodeBrickLT.o  


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libsolid_brick_27_nodes_ltensor.a


