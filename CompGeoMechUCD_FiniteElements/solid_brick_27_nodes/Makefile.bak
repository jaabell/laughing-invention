#Boris Jeremic 12April2009

include ../../Makefile.Includes
include ../../Makefile.Compilers


################### TARGETS ########################
all: TwentySevenNodeBrick.o   
	$(AR) $(ARFLAGS) ../../lib/libsolid_brick_27_nodes.a  TwentySevenNodeBrick.o  


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libsolid_brick_27_nodes.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


