#Boris Jeremic 12April2009


BASE  = /usr

# ifndef CURRENT_BASE_DIR # this to prevent $(shell pwd) from being execture over and over again
PWD := $(shell pwd)

PATH_TO_shell_element = $(PWD)


include ../../Makefile.Compilers


MACHINE_INCLUDES        = -I/usr/include 
# this file contains all the OpenSees1/ModifiedOpenSeesServices includes


 

FE_INCLUDES  = -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/matrix \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/tagged \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/element \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/material \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/material/uniaxial \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/material/nD \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/recorder \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/recorder/response \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/handler \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/actor \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/analysis \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/ \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/actor/channel \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/actor/actor \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/element \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/domain \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/domain/node \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/domain/load \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/domain/damping \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/domain/domain \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/domain/partitioner \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/domain/component \
               -I$(PATH_TO_shell_element)/../../ModifiedOpenSeesServices/actor/objectBroker \
               -I$(PATH_TO_shell_element)/../../nDarray \
               -I$(PATH_TO_shell_element)/. \
               -I../../CompGeoMechUCD_MaterialModeling/Template3Dep \
               -I../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep \



INCLUDES =  $(MACHINE_INCLUDES)  $(FE_INCLUDES)


# COMPILATION

.SUFFIXES:
.SUFFIXES:	.C .c .f .f90 .cpp .o .cpp

#
# %------------------%
# | Default command. |
# %------------------%
#
.DEFAULT:
	@$(ECHO) "Unknown target $@, try:  make help"
#
# %-----------------------------------------------%
# |  Command to build .o files from source files. |
# %-----------------------------------------------%
#

.cpp.o: 
#	@$(ECHO) Making $@ from $< $@ with $(CC++) $(C++FLAGS) $(INCLUDES) -c $<
	$(CC++) $(C++FLAGS) $(INCLUDES) -c $<


################### TARGETS ########################
all: cross_product.o ThreeNodeAndesShell.o ThreeNodeAndesBending.o ThreeNodeAndesMembrane.o FourNodeAndesShell.o
	$(AR) $(ARFLAGS) ../../lib/libshell_3_node_andes.a FourNodeAndesShell.o ThreeNodeAndesShell.o ThreeNodeAndesBending.o ThreeNodeAndesMembrane.o cross_product.o


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libshell_3_node_andes.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


