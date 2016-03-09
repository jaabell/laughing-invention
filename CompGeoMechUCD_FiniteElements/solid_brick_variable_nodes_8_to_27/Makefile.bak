#Boris Jeremic 12April2009


include ../../Makefile.Includes
include ../../Makefile.Compilers


################### TARGETS ########################
all: VariableNodeBrick.o   
	$(AR) $(ARFLAGS) ../../lib/libsolid_brick_variable_nodes_8_to_27.a  VariableNodeBrick.o


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libsolid_brick_variable_nodes_8_to_27.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


