# Nima Tafazzoli Jan 2010

include ../../Makefile.Includes
include ../../Makefile.Compilers

OBJS = NDMaterialLT.o \
       vonMisesPPLT.o \
       PisanoLT.o \
       NewPisanoLT.o \
       ElasticIsotropic3DLT.o

LIBRARY_Constitutive_Models  = ../../lib/libltensorbasedmaterials.a


################### TARGETS ########################
all: $(OBJS) 
	$(AR) $(ARFLAGS) $(LIBRARY_Constitutive_Models) $(OBJS)


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS) $(LIBRARY_Constitutive_Models)


# DO NOT DELETE THIS LINE -- make depend depends on it.
