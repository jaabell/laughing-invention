# updated by:
# Nima Tafazzoli, June 2012, UCD


include ../../Makefile.Includes
include ../../Makefile.Compilers


OBJS       = DispBeamColumn3d.o

             
################### TARGETS ########################
all:         $(OBJS)

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.

