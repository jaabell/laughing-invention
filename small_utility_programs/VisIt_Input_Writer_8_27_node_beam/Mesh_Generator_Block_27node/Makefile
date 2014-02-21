#  Written by: Nima Tafazzoli October 2010

  CC++            =  g++
  LINKER          =  g++


  C++FLAGS        = -Wall \
                    -Winline \
                    -g \
                    -D_G3DEBUG \
                    -D_LINUX \
                    -D_UNIX \
                    -D_NOGRAPHICS \
                    -DBrzi_nDarray_val 


  LINKFLAGS       = -g -Wall


# Misc
MAKE            = make
CD              = cd
ECHO            = echo
RM              = rm
RMFLAGS         = -fr
SHELL           = /bin/sh


.SUFFIXES:
.SUFFIXES:	.C .c .f .f90 .cpp .o .cpp

.DEFAULT:
	@$(ECHO) "Unknown target $@, try:  make help"

%.o : %.cpp 
	$(CC++) $(C++FLAGS) -c $<


all: Mesh_Generator

Mesh_Generator: Mesh_Generator.o
	$(LINKER) $(LINKFLAGS) Mesh_Generator.o -o Mesh_Generator



clean:
	$(RM) $(RMFLAGS) $(OBJS) *.o core.* *.err cachegrind* callgrind.* massif.* *~
	$(RM) $(RMFLAGS) Mesh_Generator nodes.txt elements.txt

