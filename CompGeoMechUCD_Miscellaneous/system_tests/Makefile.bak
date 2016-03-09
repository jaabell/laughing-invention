#include ../Makefile.Includes
include ../../Makefile.Compilers

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGETS = $(patsubst %.cpp, % , $(SRCS))

%.o: %.cpp
	$(CC++) $(C++FLAGS) $(INCLUDES) -c $< -o $@


all: $(TARGETS)


% : %.o
	$(LINKER) $(LINKFLAGS) $< $(LIBS) -o $@


###########################################################################
clean:
	$(RM) $(RMFLAGS) $(OBJS) *.o 

cleanall:
	$(RM) $(RMFLAGS) $(OBJS) *.o 
	$(RM) $(RMFLAGS) $(QuantityLIB)

