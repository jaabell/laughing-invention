include ../../../Makefile.Includes
include ../../../Makefile.Compilers

SRCS = $(wildcard *.cpp) 
OBJS = $(SRCS:.cpp=.o)
TARGETS = $(patsubst %.cpp, % , $(SRCS))

all: $(OBJS) 
	
clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS) $(LIBRARY_Constitutive_Models)
