#  Written by: Nima Tafazzoli (September 2010)

  CC++            =  g++
  LINKER          =  g++


  C++FLAGS        = -Wall \
                    -Winline \
                    -g 

  LINKFLAGS       = -g -Wall


# Misc
MAKE            = make
CD              = cd
ECHO            = echo
RM              = rm
RMFLAGS         = -fr
SHELL           = /bin/sh


.SUFFIXES:
.SUFFIXES:	.cpp .o

.DEFAULT:
	@$(ECHO) "Unknown target $@, try:  make help"

%.o : %.cpp 
	$(CC++) $(C++FLAGS) -c $<


all: DRM_1D_Shake_Input


DRM_1D_Shake_Input: DRM_1D_Shake_Input.o
	$(LINKER) $(LINKFLAGS) DRM_1D_Shake_Input.cpp \
        -o DRM_1D_Shake_Input


clean:
	$(RM) $(RMFLAGS) $(OBJS) *.o core.* *.err cachegrind* callgrind.* massif.* *~ *.vtk
	$(RM) $(RMFLAGS) DRM_1D_Shake_Input Displacements.dat Accelerations.dat

