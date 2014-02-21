#Boris Jeremic 12April2009


include ../../Makefile.Includes
include ../../Makefile.Compilers




################### TARGETS ########################
all: TwentyNodeBrickVariableGP.o   
	$(AR) $(ARFLAGS) ../../lib/libsolid_brick_20_nodes_variable_gp.a  TwentyNodeBrickVariableGP.o  


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libsolid_brick_20_nodes_variable_gp.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


