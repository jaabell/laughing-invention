#Boris Jeremic 12April2009


include ../../Makefile.Includes
include ../../Makefile.Compilers



################### TARGETS ########################
all: TwentyNodeBrick_u_p_U.o   
	$(AR) $(ARFLAGS) ../../lib/libcoupled_u-p-U_brick_20_nodes.a  TwentyNodeBrick_u_p_U.o  


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libcoupled_u-p-U_brick_20_nodes.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


