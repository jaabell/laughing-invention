
# DEBUG_MODE = DEBUG
#DEBUG_MODE = NO_DEBUG

#Compiler = GNU
# Compiler = PGI

# PROGRAMMING_MODE = SEQUENTIAL

  # PROGRAMMING_FLAG =


# BASE  = /usr

# PWD := $(shell pwd)

# PATH_TO_Constitutive_Domain = $(PWD)

include ../../Makefile.Compilers
include ../../Makefile.Includes



# MACHINE_INCLUDES        = -I/usr/include 
# # this file contains all the OpenSees1/ModifiedOpenSeesServices includes



# FE_INCLUDES  = -I$(PATH_TO_Constitutive_Domain)/../../ModifiedOpenSeesServices/matrix \
#                -I$(PATH_TO_Constitutive_Domain)/../../ModifiedOpenSeesServices/tagged \
#                -I$(PATH_TO_Constitutive_Domain)/../../ModifiedOpenSeesServices/tagged/storage \
#                -I$(PATH_TO_Constitutive_Domain)/../../ModifiedOpenSeesServices/handler \
#                -I$(PATH_TO_Constitutive_Domain)/../../ModifiedOpenSeesServices/actor/actor \
#                -I$(PATH_TO_Constitutive_Domain)/../../ModifiedOpenSeesServices/domain/component \
#                -I$(PATH_TO_Constitutive_Domain)/../../ModifiedOpenSeesServices \
#                -I$(PATH_TO_Constitutive_Domain)/../../nDarray \
#                -I$(PATH_TO_Constitutive_Domain)/../../CompGeoMechUCD_MaterialModeling \
#                -I$(PATH_TO_Constitutive_Domain)/../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep \
#                -I$(PATH_TO_Constitutive_Domain)/. 



# INCLUDES =  $(MACHINE_INCLUDES)  $(FE_INCLUDES)




OBJS =  Constitutive_Domain.o 

LIBRARY_constitutive_domain  = ../../lib/libconstitutive_domain.a



################### TARGETS ########################
all: Constitutive_Domain.o   
	$(AR) $(ARFLAGS) ../../lib/libconstitutive_domain.a  Constitutive_Domain.o  


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  ../../lib/libconstitutive_domain.a
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


