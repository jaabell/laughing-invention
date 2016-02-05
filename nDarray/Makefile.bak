#  Written: Boris Jeremic@ucdavis.edu, 17_Oct_2008

include ../Makefile.Compilers
include ../Makefile.Includes

LIBRARY_NDARRAY     = ../lib/libndarray.a  # BJ_UCD jeremic@ucdavis.edu 
NDARRAY             = BJtensor.o \
                      BJmatrix.o \
                      BJvector.o \
                      basics.o   \
                      stresst.o  \
                      straint.o  \
                      nDarray.o 


################### TARGETS ########################
all: $(NDARRAY) 
	@$(RM) $(RMFLAGS) $(LIBRARY_NDARRAY);
	$(AR) $(ARFLAGS) $(LIBRARY_NDARRAY) $(NDARRAY)

libndarray.a: $(NDARRAY)
	$(AR) $(ARFLAGS) $(LIBRARY_NDARRAY) $(NDARRAY)

clean: 
	@$(ECHO) doing: $(RM) $(RMFLAGS) $(NDARRAY) 
	@$(RM) $(RMFLAGS) $(NDARRAY) *.o

cleanall: 
	@$(ECHO) doing: $(RM) $(RMFLAGS) $(NDARRAY) 
	@$(RM) $(RMFLAGS) $(NDARRAY)
	@$(ECHO) doing: $(RM) $(RMFLAGS) $(LIBRARY_NDARRAY) 
	@$(RM) $(RMFLAGS) $(LIBRARY_NDARRAY) 

# DO NOT DELETE THIS LINE -- make depend depends on it.
