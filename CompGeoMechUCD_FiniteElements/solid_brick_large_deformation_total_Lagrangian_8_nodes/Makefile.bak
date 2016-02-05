# updated by:
# Nima Tafazzoli, November 2009, UCD

include ../../Makefile.Compilers

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../../ModifiedOpenSeesServices

include ../../ModifiedOpenSeesServices/Makefile.def



OBJS  = TotalLagrangianFD20NodeBrick.o \
	TotalLagrangianFD8NodeBrick.o \
	TclTLFD20NodeBrickCommand.o

all:         $(OBJS)

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o 
	@$(RM) $(RMFLAGS) *.o
	@$(RM) $(RMFLAGS) .inslog*
	@$(RM) $(RMFLAGS) tca.map

spotless: clean					   

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.

