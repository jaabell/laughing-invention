# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..


include ../../../Makefile.Includes
include ../../../Makefile.Compilers

OBJS       = SectionForceDeformation.o \
             ElasticSection3d.o \
             GenericSection1d.o \
             SectionAggregator.o \
             ElasticPlateSection.o \
             ElasticMembranePlateSection.o \
             MembranePlateFiberSection.o \
             FiberSection3d.o \
             Bidirectional.o \
             FiberSectionGJ.o \
             Isolator2spring.o \
             ElasticShearSection3d.o
#             TclModelBuilderSectionCommand.o \
#              ElasticSection2d.o \
#              FiberSection2d.o \

all:         $(OBJS)
	@$(CD) repres; $(MAKE);
	@$(CD) fiber; $(MAKE);
	@$(CD) matrixutil; $(MAKE);
# 	@$(CD) yieldSurface; $(MAKE);

# Miscellaneous

tidy:   
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless
	@$(CD) repres; $(MAKE) wipe;
	@$(CD) fiber; $(MAKE) wipe;
	@$(CD) matrixutil; $(MAKE) wipe;
# 	@$(CD) yieldSurface; $(MAKE) wipe;

# DO NOT DELETE THIS LINE -- make depends on it.
