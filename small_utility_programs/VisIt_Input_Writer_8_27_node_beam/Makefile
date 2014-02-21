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


all: VisIt_Input_Writer


VisIt_Input_Writer: VisIt_Input_Writer.o
	$(LINKER) $(LINKFLAGS) VisIt_Input_Writer.cpp \
        -o VisIt_Input_Writer


clean:
	$(RM) $(RMFLAGS) $(OBJS) *.o core.* *.err cachegrind* callgrind.* massif.* *~ *.vtk
	$(RM) $(RMFLAGS) VisIt_Input_Writer

