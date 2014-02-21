
include ../../Makefile.Includes
include ../../Makefile.Compilers


################### TARGETS ########################
all: TwentySevenNodeBrickVariableGP.o   
	$(AR) $(ARFLAGS) ../../lib/libsolid_brick_27_nodes_variable_gp.a  TwentySevenNodeBrickVariableGP.o  


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libsolid_brick_27_nodes_variable_gp.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


