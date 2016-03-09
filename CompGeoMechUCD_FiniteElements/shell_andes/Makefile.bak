#Boris Jeremic 12April2009


include ../../Makefile.Includes
include ../../Makefile.Compilers


################### TARGETS ########################
all: cross_product.o ThreeNodeAndesShell.o ThreeNodeAndesBending.o ThreeNodeAndesMembrane.o FourNodeAndesShell.o
	$(AR) $(ARFLAGS) ../../lib/libshell_3_node_andes.a FourNodeAndesShell.o ThreeNodeAndesShell.o ThreeNodeAndesBending.o ThreeNodeAndesMembrane.o cross_product.o


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libshell_3_node_andes.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


