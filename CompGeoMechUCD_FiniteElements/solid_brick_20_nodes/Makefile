#Boris Jeremic 12April2009


include ../../Makefile.Includes
include ../../Makefile.Compilers




################### TARGETS ########################
all: TwentyNodeBrick.o   
	$(AR) $(ARFLAGS) ../../lib/libsolid_brick_20_nodes.a  TwentyNodeBrick.o  


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libsolid_brick_20_nodes.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


