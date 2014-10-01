#Jose Abell November 4, 2013


include ../../Makefile.Includes
include ../../Makefile.Compilers


################### TARGETS ########################
all: ShearBeamLT.o   
	$(AR) $(ARFLAGS) ../../lib/libnonlinear_shear_beam.a  ShearBeamLT.o  


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libnonlinear_shear_beam.a


