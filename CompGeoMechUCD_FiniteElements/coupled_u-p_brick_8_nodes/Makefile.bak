#Boris Jeremic 12April2009

include ../../Makefile.Includes
include ../../Makefile.Compilers



OBJS =  EightNode_Brick_u_p.o 

LIBRARY_coupled_u-p_brick_8_nodes  = ../../lib/libcoupled_u-p_brick_8_nodes.a



################### TARGETS ########################
all: EightNode_Brick_u_p.o   
	$(AR) $(ARFLAGS) ../../lib/libcoupled_u-p_brick_8_nodes.a  EightNode_Brick_u_p.o  


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libcoupled_u-p_brick_8_nodes.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


