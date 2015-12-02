# updated by:
# Nima Tafazzoli, June 2012, UCD


include ../../Makefile.Includes
include ../../Makefile.Compilers


OBJS       = CorotCrdTransf3d.o  CrdTransf.o  LinearCrdTransf3d.o  PDeltaCrdTransf3d.o


             
################### TARGETS ########################
all:         $(OBJS)


test: $(UCB_ELEMENTS) CantileverPureBending.o 
	$(CC++) CantileverPureBending.o DispBeamColumn3d.o CorotCrdTransf3d.o CrdTransf.o $(LIBS) -o CantileverPureBending 

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.

