#TEACHING_MODE =

DEBUG_MODE = DEBUG
#DEBUG_MODE = NO_DEBUG


# ###################################################

  CC++            =  g++
  LINKER          =  g++


ifeq ($(DEBUG_MODE), NO_DEBUG)
  C++FLAGS        = -Wall \
                    -Winline \
                    -O3 \
                    -finline \
                    -finline-functions \
                    -fkeep-inline-functions \
                    -fdefault-inline \
                    -ffloat-store \
                    -ffast-math \
                    -D_LINUX \
                    -D_UNIX \
                    -D_NOGRAPHICS \
                    -DBrzi_nDarray_val


  LINKFLAGS       = -O3
else
  C++FLAGS        = -Wall \
                    -Winline \
                    -g \
                    -D_G3DEBUG \
                    -D_LINUX \
                    -D_UNIX \
                    -D_NOGRAPHICS \
                    -DBrzi_nDarray_val 


  LINKFLAGS       = -g -Wall
endif

# Misc
MAKE            = make
CD              = cd
ECHO            = echo
RM              = rm
RMFLAGS         = -fr
SHELL           = /bin/sh



INCLUDES  = -I../../ModifiedOpenSeesServices/matrix \
            -I../../ModifiedOpenSeesServices/tagged \
            -I../../ModifiedOpenSeesServices/material \
            -I../../ModifiedOpenSeesServices/material/nD \
            -I../../ModifiedOpenSeesServices/handler \
            -I../../ModifiedOpenSeesServices/actor \
            -I../../ModifiedOpenSeesServices/actor/channel \
            -I../../ModifiedOpenSeesServices/actor/message \
            -I../../ModifiedOpenSeesServices/actor/objectBroker \
            -I../../ModifiedOpenSeesServices/actor/actor \
            -I../../ModifiedOpenSeesServices/domain	\
            -I../../ModifiedOpenSeesServices/domain/domain	\
            -I../../ModifiedOpenSeesServices/domain/domain/single	\
            -I../../ModifiedOpenSeesServices/domain/component	\
            -I../../ModifiedOpenSeesServices/domain/component/storage	\
            -I../../ModifiedOpenSeesServices \
            -I../../nDarray \
            -I../../CompGeoMechUCD_MaterialModeling/Template3Dep \
            -I../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep \
            -I../../CompGeoMechUCD_MaterialModeling/FiniteDeformation \
            -I../../CompGeoMechUCD_MaterialModeling/FiniteDeformation/fdFlow \
            -I../../CompGeoMechUCD_MaterialModeling/FiniteDeformation/fdYield \
            -I../../CompGeoMechUCD_MaterialModeling/FiniteDeformation/fdEvolution \
            -I../../CompGeoMechUCD_FiniteElements/coupled_u-p_brick_8_nodes \
            -I../../CompGeoMechUCD_FiniteElements/coupled_u-p_large_deformation_brick_8_nodes \
            -I../../CompGeoMechUCD_FiniteElements/coupled_u-p-U_brick_20_nodes \
            -I../../CompGeoMechUCD_FiniteElements/coupled_u-p-U_brick_8_nodes \
            -I../../CompGeoMechUCD_FiniteElements/solid_brick_20_nodes \
            -I../../CompGeoMechUCD_FiniteElements/solid_brick_27_nodes \
            -I../../CompGeoMechUCD_FiniteElements/solid_brick_8_nodes \
            -I../../CompGeoMechUCD_FiniteElements/solid_brick_large_deformation_total_Lagrangian_20_nodes \
            -I../../CompGeoMechUCD_FiniteElements/solid_brick_large_deformation_total_Lagrangian_8_nodes \



%.o : %.cpp
	$(CC++) $(C++FLAGS) $(INCLUDES) -c $<


all:  Mesh_Generator


Mesh_Generator:  Mesh_Generator.o
	$(LINKER) $(LINKFLAGS) Mesh_Generator.o \
                         ../../lib/libMOSS.a \
                         ../../lib/libndarray.a \
                         ../../lib/liblapack.a \
                         ../../lib/libblas.a \
                        -L$(BASE)/lib64 \
                        -L$(HOME)/lib \
                        -lm \
                        -ldl \
                        -lg2c \
                        -lieee \
                        -lgfortran \
                        -lc \
                         -o Mesh_Generator
                        
                     


clean:
	$(RM) $(RMFLAGS) $(OBJS) *.o core.* *.err cachegrind* callgrind.* massif.* *~
	$(RM) $(RMFLAGS) Mesh_Generator nodes.txt elements.txt


