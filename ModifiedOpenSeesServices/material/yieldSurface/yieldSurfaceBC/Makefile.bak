# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../../..


include ../../../../Makefile.Includes
include ../../../../Makefile.Compilers

OBJS       = NullYS2D.o \
             Orbison2D.o \
             YieldSurface_BC2D.o \
             YieldSurface_BC.o
#             TclModelBuilderYieldSurfaceBCCommand.o
#              Attalla2D.o \
#              ElTawil2D.o \
#              ElTawil2DUnSym.o \
#              Hajjar2D.o \


all:         $(OBJS)

# Miscellaneous

tidy:   
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.
