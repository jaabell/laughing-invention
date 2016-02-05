#Boris Jeremic 06Mar2009

DEBUG_MODE = DEBUG
#DEBUG_MODE = NO_DEBUG

#Compiler = GNU
# Compiler = PGI

# BASE  = /usr

# PATH_TO_Template3Dep       = $(PWD)

include ../../Makefile.Compilers
include ../../Makefile.Includes



# MACHINE_INCLUDES        = -I/usr/include 
# # this file contains all the OpenSees1/ModifiedOpenSeesServices includes


# FE_INCLUDES  = -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/matrix \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/tagged \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/material \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/material/nD \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/recorder \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/handler \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/actor \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/analysis \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/ \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/actor/channel \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/actor/actor \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/element \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/domain \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/domain/domain \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/domain/partitioner \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/domain/component \
#                -I$(PATH_TO_Template3Dep)/../../ModifiedOpenSeesServices/actor/objectBroker \
#                -I$(PATH_TO_Template3Dep)/../../nDarray \
#                -I$(PATH_TO_Template3Dep)/. \



# INCLUDES =  $(MACHINE_INCLUDES)  $(FE_INCLUDES)


# COMPILATION

# .SUFFIXES:
# .SUFFIXES:	.C .c .f .f90 .cpp .o .cpp

# #
# # %------------------%
# # | Default command. |
# # %------------------%
# #
# .DEFAULT:
# 	@$(ECHO) "Unknown target $@, try:  make help"
# #
# # %-----------------------------------------------%
# # |  Command to build .o files from source files. |
# # %-----------------------------------------------%
# #

# .cpp.o: 
# #	@$(ECHO) Making $@ from $< $@ with $(CC++) $(C++FLAGS) $(INCLUDES) -c $<
# 	$(CC++) $(C++FLAGS) $(INCLUDES) -c $<


#include ../../Makefile.def



OBJS  = Template3Dep.o \
        MatPoint3D.o \
        CAM_PS.o  \
        CAM_YS.o \
        DP_PS.o  \
        DP_YS.o \
        EL_LEeq.o \
        EL_LEij.o \
        EL_NLEp.o\
        EL_NLEeq.o\
        EL_NLEij.o\
        EL_NLEijMD.o\
        EL_S.o	  \
        EL_T.o	  \
        EPState.o \
        VM_PS.o	  \
        VM_YS.o	  \
        YS.o	  \
        PS.o     \
        MD_PS01.o	  \
        MD_PS.o	  \
        DP_YS01.o	  \
        MD_YS.o	  \
        Tri_a_fail_crit_YS.o \
        RMC01.o      \
        RMC01_PS.o   \
        RMC01_YS.o   

#        TclNewTemplate3Dep.o \

LIBRARY_Template_3D_EP  = ../../lib/libtemplate3dep.a


################### TARGETS ########################
all: $(OBJS) 
	$(AR) $(ARFLAGS) $(LIBRARY_Template_3D_EP) $(OBJS)

#libmatmod.a: $(OBJS)
#	$(AR) $(ARFLAGS) $(Template_3D_EP) $(OBJS)


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS) $(LIBRARY_Template_3D_EP)



# DO NOT DELETE THIS LINE -- make depend depends on it.




