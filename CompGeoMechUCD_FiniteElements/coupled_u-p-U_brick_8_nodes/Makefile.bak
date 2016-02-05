#Boris Jeremic 12April2009


include ../../Makefile.Includes
include ../../Makefile.Compilers



MACHINE_INCLUDES        = -I/usr/include 
# this file contains all the OpenSees1/ModifiedOpenSeesServices includes


################### TARGETS ########################
all: EightNodeBrick_u_p_U.o   
	$(AR) $(ARFLAGS) ../../lib/libcoupled_u-p-U_brick_8_nodes.a  EightNodeBrick_u_p_U.o  


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libcoupled_u-p-U_brick_8_nodes.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


