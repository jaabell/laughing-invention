# Nima Tafazzoli Jan 2010

include ../../Makefile.Includes
include ../../Makefile.Compilers


OBJS =  vonMisesPP.o \
        vonMisesIH.o \
        vonMisesKH.o \
        vonMisesLinearKH.o \
        DruckerPragerPP.o \
        DruckerPragerIH.o \
        DruckerPragerKH.o \
        CamClay.o \
        Pisano_model.o


LIBRARY_Constitutive_Models  = ../../lib/libaccelerated3dep.a


################### TARGETS ########################
all: $(OBJS) 
	$(AR) $(ARFLAGS) $(LIBRARY_Constitutive_Models) $(OBJS)



clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS) $(LIBRARY_Constitutive_Models)


# DO NOT DELETE THIS LINE -- make depend depends on it.




