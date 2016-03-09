#Jose Abell (2013)

PWD := $(shell pwd)
PATH_TO_CompGeoMechUCD_Miscellaneous = $(PWD)

include ../Makefile.Compilers

all:
	$(CD) $(PATH_TO_CompGeoMechUCD_Miscellaneous)/units; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_Miscellaneous)/output; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_Miscellaneous)/timing; $(MAKE);

clean:
	$(CD) $(PATH_TO_CompGeoMechUCD_Miscellaneous)/units; $(MAKE) clean;

cleanall:
	$(CD) $(PATH_TO_CompGeoMechUCD_Miscellaneous)/units; $(MAKE) cleanall;